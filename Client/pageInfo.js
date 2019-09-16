function initShiftGauge(){
        var opts = {
        angle: 0.04, // The span of the gauge arc
        lineWidth: 0.28, // The line thickness
        radiusScale: 1, // Relative radius
        pointer: {
        length: 0.51, // // Relative to gauge radius
        strokeWidth: 0.035, // The thickness
        color: '#000000' // Fill color
        },
        limitMax: false,     // If false, max value increases automatically if value > maxValue
        limitMin: false,     // If true, the min value of the gauge will be fixed
        colorStart: '#6FADCF',   // Colors
        colorStop: '#8FC0DA',    // just experiment with them
        strokeColor: '#E0E0E0',  // to see which ones work best for you
        generateGradient: true,
        highDpiSupport: true,     // High resolution support
        // renderTicks is Optional
        renderTicks: {
        divisions: 3,
        divWidth: 1.8,
        divLength: 0.7,
        divColor: '#333333',
        subDivisions: 5,
        subLength: 0.5,
        subWidth: 0.9,
        subColor: '#001409'
        }
        
    }
    var target = document.getElementById('shiftGauge'); // your canvas element
    var gauge = new Gauge(target).setOptions(opts); // create sexy gauge!
    gauge.maxValue = 3; // set max gauge value
    gauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
    gauge.animationSpeed = 71; // set animation speed (32 is default value)
    gauge.set(0); // set actual value
    return gauge;
}
function initSpeedGauge(){
    var opts = {
    angle: 0.04, // The span of the gauge arc
    lineWidth: 0.28, // The line thickness
    radiusScale: 1, // Relative radius
    pointer: {
    length: 0.51, // // Relative to gauge radius
    strokeWidth: 0.035, // The thickness
    color: '#000000' // Fill color
    },
    limitMax: false,     // If false, max value increases automatically if value > maxValue
    limitMin: false,     // If true, the min value of the gauge will be fixed
    colorStart: '#6FADCF',   // Colors
    colorStop: '#8FC0DA',    // just experiment with them
    strokeColor: '#E0E0E0',  // to see which ones work best for you
    generateGradient: true,
    highDpiSupport: true,     // High resolution support
    // renderTicks is Optional
    renderTicks: {
    divisions: 3,
    divWidth: 1.8,
    divLength: 0.7,
    divColor: '#333333',
    subDivisions: 5,
    subLength: 0.5,
    subWidth: 0.9,
    subColor: '#001409'
    }
    
}
var target = document.getElementById('speedGauge'); // your canvas element
var gauge = new Gauge(target).setOptions(opts); // create sexy gauge!
gauge.maxValue = 3; // set max gauge value
gauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
gauge.animationSpeed = 71; // set animation speed (32 is default value)
gauge.set(0); // set actual value
return gauge;
}

var ctx = document.getElementById('throttleGraph').getContext('2d');
var throttleChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Throttle',
            data: [0],
            backgroundColor: [
                'rgba(255, 99, 132, 0.2)'
            ],
            borderColor: [
                'rgba(255, 99, 132, 1)'
            ],
            borderWidth: 1
        }]
    },
    options: {
        maintainAspectRatio: false,
        animation: false,
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true,
                    suggestedMin: 0,
                    suggestedMax: 1,
                }
            }]
        }
    }
});
var i = 0
/*

Used to test the chart refresh speed

window.setInterval(() => {
    myChart.data.labels.push(i.toString());
    myChart.data.datasets[0].data.push(Math.ceil(Math.random() * 10));
    myChart.update();
    if (i>20) {
        myChart.data.datasets[0].data.shift()
        myChart.data.labels.shift();
    }
    i++
}, 50);
*/

function pushThrottleData(chart,data) {
    chart.data.labels.push(i.toString());
    chart.data.datasets[0].data.push(data);
    chart.update();
    if (i>20) {
        chart.data.datasets[0].data.shift()
        chart.data.labels.shift();
    }
    i++
}

