var path = require('path');
var fs   = require('fs');
//var live = require('./teste');
var loadedConfig = undefined;
var mpd_name;
var path_segments;

var defaultConfig = {
  inputFile: undefined,
  // segmentDir: path.sep + path.join(live.path_segments, 'dashts'),
  // outputDir: path.sep + path.join(live.path_segments, 'dash'),
  segmentDir: path.sep + path.join('/Users/Josue/Downloads/proxy-master/site', 'dashts'),
  outputDir: path.sep + path.join('/Users/Josue/Downloads/proxy-master/site', 'dash'),
  mp4box: 'MP4Box',
  ffmpeg: 'ffmpeg'
}

var defaultEncoding = {
  commandPrefix: [ '-re', '-i', '-', '-threads', '30','-y' ],
  representations: {
  //SE NÃO HOUVER AUDIO, COMENTAR O CODIGO DE AUDIO ABAIXO 
  
  	/*audio: [
      '-map', '0:1', '-vn', '-acodec', 'aac', '-strict', '-2', //'-ar', '44100', '-ac', '2','-ab','128000', 
      '-f', 'segment', '-segment_time', '1', '-segment_format', 'mpegts'
    ],*/
  //ATE AQUI

  //ALTERAR RESOLUÇÃO EM -s PARA RESOLUÇÃO DO VIDEO E -keyint_min 2*FPS / -g FPS / -r FPS
    video: [
      '-map', '0:0', '-vcodec', 'libx264', '-vprofile', 'high', '-preset', 'ultrafast',
      /*'-s', '3996x2160', '-vb', '30000k','-minrate','100000k', '-bufsize', '250000k', '-maxrate', '40000k',*/
      '-level', '5.1', '-keyint_min', '48', '-g', '24','-r','24', '-sc_threshold', '0', '-an',
      //'-bsf', 'h264_mp4toannexb', '-flags', '-global_header', '-tune','zerolatency',
      '-f', 'segment', '-segment_time', '1', '-segment_format', 'mpegts'//, '-probesize', '100000', '-analyzeduration', '0'
    ]
  }
}

var defaultPackaging = {
  //ALTERAR -base-url PARA O IP DO SERVER DASH
  mp4box_opts: [
    '-dash-ctx', path.join(defaultConfig.outputDir, 'dash-live.txt'), '-dash', '1000', '-rap', '-ast-offset', '-1800', 
    '-no-frags-default', '-bs-switching', 'no', '-url-template', '-time-shift', '1800', '-mpd-refresh', '1','-profile','dashavc264:live',
    '-mpd-title', 'MPEG-DASH live stream', '-mpd-info-url', 'http://ebu.io/', '-segment-name','$RepresentationID$', '-out', path.join(defaultConfig.outputDir, 'live'),
    '-dynamic', '-subsegs-per-sidx', '-1'
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
  mpd_name = mpd;
  path_segments = path_seg;
  var aux;
  if (configFile) {
    // Attempt to load config from file, exit if fails
    try {
      var data = fs.readFileSync(configFile, 'utf8');
      loadedConfig = JSON.parse(data);      
    } catch(e) {
      console.log("Error: invalid data found while trying to read config file. Please make sure it exists and contains valid JSON data.");
      process.exit(1);
    }
    merge(defaultConfig, loadedConfig);
  }
  defaultConfig.inputFile = inputFile;
  defaultConfig.segmentDir = path.sep + path.join(path_seg, 'dashts');
  defaultConfig.outputDir = path.sep + path.join(path_seg, 'dash');

  for(i = 0; i<defaultPackaging.mp4box_opts.length; i++){

    if(defaultPackaging.mp4box_opts[i] === "-out"){
      aux = defaultConfig.outputDir;
      defaultPackaging.mp4box_opts[++i] = aux+"/"+mpd;
    }
  }
  console.log(defaultPackaging.mp4box_opts);
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
