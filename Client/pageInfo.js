function initGauge(){
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
    var target = document.getElementById('foo'); // your canvas element
    var gauge = new Gauge(target).setOptions(opts); // create sexy gauge!
    gauge.maxValue = 3; // set max gauge value
    gauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
    gauge.animationSpeed = 71; // set animation speed (32 is default value)
    gauge.set(0); // set actual value
    return gauge;
}