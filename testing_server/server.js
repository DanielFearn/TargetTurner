const http = require('http');

const express = require('express');
const app = express();

const port = 80;

const numTargets = 10;


app.listen(port);
console.log('Listening on port ' + port + '\n\n');




app.use('/', express.static('interface'));

app.get('/upload/*', uploadHandler);

app.get('/*', commandHandler);

function uploadHandler(req, res) {

    var sequenceText = req.url.substr(8);

    var sequence = sequenceText.split(',').map((x) => {return parseFloat(x);});

    var sequenceLength = sequence.shift();

    console.log('Sequence received length ' + sequenceLength);

    var firstLine = '| ';
    var seperatorLine = '+'
    for (var h = 0; h < numTargets; h++) {
        firstLine += h.toString() + ' | ';
        seperatorLine += '---+';
    }

    firstLine += 'DUR |';
    seperatorLine += '-----+';

    console.log(seperatorLine);
    console.log(firstLine);
    console.log(seperatorLine);

    for (var i = 0; i < sequenceLength; i++) {

        var line = '| ';

        for(var j = 0; j < numTargets; j++) {
            line += sequence[i*(numTargets+1) + j].toString() + ' | ';
        }


        line += sequence[i*(numTargets+1) + j].toFixed(1) + ' |'; // print duration to 1.d.p

        console.log(line);
    }


    res.end('OK');
}

function commandHandler(req, res) {

    console.log('Command received: '+ req.url.substr(1)); 
    res.end('Received');
}
