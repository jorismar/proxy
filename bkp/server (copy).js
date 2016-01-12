//var app = require('express')();
var express = require('express');
var app = express();
var path = require('path');
var server = require('http').createServer(app);

var port = process.env.PORT || 3000;
server.listen(port);
console.log("Server running on port: " + port);

app.get('/', function(req, res) {
  res.sendfile(__dirname + '/index.html');
});

app.get('/lavid.png', function(req, res) {
  res.sendfile(__dirname + '/lavid.png');
});

app.get('/lavid.jpg', function(req, res) {
  res.sendfile(__dirname + '/lavid.jpg');
});

app.get('/dash.all.js', function(req, res) {
  res.sendfile(__dirname + '/dash.all.js');
});

app.get('/video.mp4', function(req, res) {
  res.sendfile(__dirname + '/video.mp4');
});

app.get('/favicon.ico', function(req, res) {
  res.sendfile(__dirname + '/favicon.ico');
});

app.use(express.static(path.join(__dirname, 'dash')));

