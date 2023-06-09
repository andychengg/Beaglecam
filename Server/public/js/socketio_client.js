"use strict";

// using example code from: 10-NodeJs-ExampleCode/10-UdpRelay/public/javascripts/udp_ui.js

var socket = io.connect();

$(document).ready(function() {

    const ctx = document.getElementById('myChart');

    const data = {
        labels: [],
        datasets: [{
            label: 'Night mode',
            data: [],
            fill: false,
            borderColor: 'rgb(75, 192, 192)',
            tension: 0.1
        }]
    };
    
    var liveChart = new Chart(ctx, {
        type: 'line',
        data: data,
        options: {
            legend: {
                display: false
            },
            tooltips: {
                callbacks: {
                label: function(tooltipItem) {
                        return tooltipItem.yLabel;
                }
                }
            }
        }
    });

    $.get('/data', function (data, status) {
        console.log('fetch');
        console.log(data);

        updateChart(data.dangerLabels, data.dangerPoints);
        updateDangerLevel(data.dangerLevel);
    });

	socket.on('data', (response) => {
		// todo: update ui
        let data = JSON.parse(response);
        console.log(data);

        updateChart(data.dangerLabels, data.dangerPoints);
        updateDangerLevel(data.dangerLevel);
	});


    setInterval(updateTime, 1000);

    function updateTime() {
        let time = document.getElementById('videoMetadataOverlay');
        time.innerHTML = new Date().toLocaleString('en-US');
    }

    function updateChart(labels, datapoints) {
        liveChart.data.labels = labels;
        liveChart.data.datasets[0].data = datapoints;
        liveChart.update();
    }
    
    function updateDangerLevel(dangerLevel) {
        let line = document.getElementById('myLine');
        line.style.width = numToPercent(dangerLevel);
        line.style.backgroundColor = numToColor(dangerLevel);
        document.getElementById('lineTitle').innerHTML = 'Danger Level: ' + numToPercent(dangerLevel);
    }
});


function numToColor(r) {
    if (r < 0.3) {
        return 'green'
    } else if (r >= 0.3 && r < 0.55) {
        return 'yellow'
    } else {
        return 'red'
    }
}
function numToPercent(r) {
    return (r * 100).toFixed(2).toString() + '%';
}

// Convertion logic source:
// https://stackoverflow.com/questions/37096367/how-to-convert-seconds-to-minutes-and-hours-in-javascript
function formatTime(totalSeconds) {
	const hours = Math.floor(totalSeconds / 3600) || 0;
	const minutes = Math.floor(totalSeconds % 3600 / 60) || 0;
	const seconds = Math.floor(totalSeconds % 3600 % 60) || 0;

	return hours + ':' + minutes + ':' + seconds;
}