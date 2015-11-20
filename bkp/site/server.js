var app = require('express')();
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

app.get('/video.ogg', function(req, res) {
  res.sendfile(__dirname + '/video.ogg');
});

