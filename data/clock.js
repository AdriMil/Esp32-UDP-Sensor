// clock.js

function updateClock() {
    // Get current date and hour
    var now = new Date();

    // Retrieve hour-minutes-seconds
    var hours = now.getHours().toString().padStart(2, '0');
    var minutes = now.getMinutes().toString().padStart(2, '0');
    var seconds = now.getSeconds().toString().padStart(2, '0');

    // Retrieve date (day, month, year)
    var day = now.getDate().toString().padStart(2, '0');
    var month = (now.getMonth() + 1).toString().padStart(2, '0');
    var year = now.getFullYear();

    // Format date and hour
    var timeString = hours + ":" + minutes + ":" + seconds;
    var dateString = day + "/" + month + "/" + year;

    // Display date and hours with id="clock"
    document.getElementById('clock').innerHTML = dateString + " " + timeString;
}

//Update hours every seconds
function startClock() {
    updateClock()
    setInterval(updateClock, 1000);
}
