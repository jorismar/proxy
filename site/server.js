//var app = require('express')();
var express = require('express');
var app = express();
var path = require('path');
var server = require('http').createServer(app);

var port = process.env.PORT || 3000;
server.listen(port);
console.log("Server running on port: " + port);

app.use(express.static(path.join(__dirname, './')));
//app.use(express.static(path.join(__dirname, 'dash')));

