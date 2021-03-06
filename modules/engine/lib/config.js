var path = require('path');
var fs   = require('fs');
var loadedConfig = undefined;
var ffmpeg_out_dir = 'dashts';
var mp4box_out_dir = 'dash';

var defaultConfig = {
  inputFile: undefined,
  segmentDir: path.join('/tmp/dashserver', ffmpeg_out_dir),
  outputDir: path.join('/tmp/dashserver', mp4box_out_dir),
  mp4box: 'MP4Box',
  ffmpeg: 'ffmpeg'
}

var defaultEncoding = {
  commandPrefix: [ '-re', '-i', '-', '-threads', '30','-y' ],
  representations: {
  	audio: [
      '-map', '0:0', '-vn', '-acodec', 'aac', '-strict', '-2', '-ar', '48000', '-ac', '2', 
      '-f', 'segment', '-segment_time', '1', '-segment_format', 'mpegts'
    ],

    video: [
      '-map', '0:1', '-vcodec', 'libx264', '-vprofile', 'high', '-preset', 'ultrafast',
      '-level', '5.1', '-keyint_min', '48', '-g', '24','-r','24', '-sc_threshold', '0', '-an',
      '-f', 'segment', '-segment_time', '1', '-segment_format', 'mpegts'
    ]
  }
}

var defaultPackaging = {
  mp4box_opts: [
    '-dash-ctx', path.join(defaultConfig.outputDir, 'dash-live.txt'), '-out', path.join(defaultConfig.outputDir, 'live'),
	'-dash', '1000', '-rap', '-ast-offset', '12', '-no-frags-default', '-bs-switching', 'no', '-min-buffer', '1000',
	'-url-template', '-time-shift', '10', '-mpd-refresh', '1', '-mpd-title', 'MPEG-DASH live stream', '-single-segment',
	'-mpd-info-url', 'http://ebu.io/', '-segment-name','$RepresentationID$', '-dynamic', '-subsegs-per-sidx', '-1'
  ]
}

defaultConfig.encoding = defaultEncoding;
defaultConfig.packaging = defaultPackaging;

function getRepresentationKeys() {
  return Object.keys(defaultConfig.encoding.representations);
}

function getRepresentationKeyForFile(segment) {
  var keys = getRepresentationKeys();
  var result = undefined;
  keys.forEach(function(key) {
    var file = segment.split(path.sep).pop();
    if (file.indexOf(key) == 0) {
      result = key;
    }
  })
  return result;
}

function merge(orig, custom) {
  for (key in custom) {
    orig[key] = custom[key];
  }
}

function load(inputFile, configFile, mpd, path_seg) {
  defaultConfig.inputFile = inputFile;

  if((mpd || path_seg) != undefined){
    defaultConfig.segmentDir = path.join(path_seg, ffmpeg_out_dir);
    defaultConfig.outputDir = path.join(path_seg, mp4box_out_dir);

	defaultPackaging.mp4box_opts[1] = path.join(defaultConfig.outputDir, 'dashlive.txt');
	defaultPackaging.mp4box_opts[3] = path.join(defaultConfig.outputDir, mpd);
  }
}

var Config = {};
Config.load = load;
Config.getRepresentationKeyForFile = getRepresentationKeyForFile;
Object.defineProperty(Config, 'inputFile',  { get: function() { return defaultConfig.inputFile; }});
Object.defineProperty(Config, 'segmentDir', { get: function() { return defaultConfig.segmentDir; }});
Object.defineProperty(Config, 'outputDir',  { get: function() { return defaultConfig.outputDir; }});
Object.defineProperty(Config, 'ffmpeg',     { get: function() { return defaultConfig.ffmpeg; }});
Object.defineProperty(Config, 'mp4box',     { get: function() { return defaultConfig.mp4box; }});
Object.defineProperty(Config, 'encoding',   { get: function() { return defaultConfig.encoding; }});
Object.defineProperty(Config, 'packaging',  { get: function() { return defaultConfig.packaging; }});
Object.defineProperty(Config, 'representationKeys', { get: function() { return getRepresentationKeys(); }});
Object.defineProperty(Config, 'representationCount', { get: function() { return getRepresentationKeys().length; }});

module.exports = Config;
