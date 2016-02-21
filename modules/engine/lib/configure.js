{
  segmentDir: segmentDir: path.sep + path.join('/Users/Josue/Downloads/proxy-master/site', 'dashts'),
  outputDir: path.sep + path.join('/Users/Josue/Downloads/proxy-master/site', 'dash'),
  mp4box: 'MP4Box',
  ffmpeg: 'ffmpeg',
  encoding: {
    commandPrefix: [ '-re', '-i', '-', '-threads', '30', '-y' ],
    representations: {
      audio: [
        '-map', '0:1', '-vn', '-acodec', 'aac', '-strict', '-2', '-f', 'segment', '-segment_time', '1', '-segment_format', 
        'mpegts'
      ],
      video: [
         '-map', '0:0', '-vcodec', 'libx264', '-vprofile', 'high', '-preset', 'ultrafast',
         '-level', '5.1', '-keyint_min', '48', '-g', '24','-r','24', '-sc_threshold', '0', '-an',
         '-f', 'segment', '-segment_time', '1', '-segment_format', 'mpegts'
      ]
    }
  },
  packaging: {
    mp4box_opts: [
      '-dash-ctx', path.join(defaultConfig.outputDir, 'dash-live.txt'), '-dash', '1000', '-rap', '-ast-offset', '-1800', '-no-frags-default', 
      '-bs-switching', 'no'/*, '-min-buffer', '1000'*/, '-url-template', '-time-shift', '1800', '-mpd-refresh', '1','-profile','dashavc264:live',
      '-mpd-title', 'MPEG-DASH live stream', '-mpd-info-url', 'http://ebu.io/', '-segment-name',
      '$RepresentationID$', '-out', path.join(defaultConfig.outputDir, 'live'), '-dynamic', '-subsegs-per-sidx', '-1'
    ]
  }
}