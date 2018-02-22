var closedImage = document.getElementById("closed-image");
var openedImage = document.getElementById("opened-image");
var changingImage = document.getElementById("changing-image");

document.getElementById("timer-button").addEventListener('click', timer);
document.getElementById("to-login-again").addEventListener('click', toLogin);
document.getElementById("to-timer-page").addEventListener('click', toTimer);
document.getElementById("to-control-again").addEventListener('click', toControl);



function toControl() {

    if(motorStatus === "garageClosed") {
        document.getElementById("indicator").innerHTML = "Status: Closed";
        closedPicture();

    } else if (motorStatus === "garageOpened") {
        document.getElementById("indicator").innerHTML = "Status: Opened";
        openedPicture();
    }

    //console.log("I AM HERE!");
    document.getElementById("control-screen-main-page").hidden = false;
    document.getElementById("login-main-page").hidden = true;
    document.getElementById("timer-page").hidden = true;
}



function toLogin() {
    document.getElementById("control-screen-main-page").hidden = true;
    document.getElementById("login-main-page").hidden = false;
    document.getElementById("timer-page").hidden = true;
}

function toTimer() {
    document.getElementById("control-screen-main-page").hidden = true;
    document.getElementById("login-main-page").hidden = true;
    document.getElementById("timer-page").hidden = false;
}

document.getElementById('autofeature').addEventListener('click', changeFeature);

var disabled = false;

function changeFeature() {
    disabled = !disabled;
    if(disabled === true) {
        document.getElementById('autofeature').value = "Enable";
    } else {
        document.getElementById('autofeature').value = "Disable";
    }
}

function timer() {
    //console.log(document.getElementById("timersetbox").value * 1000);

    if(document.getElementById("timersetbox").value < 101 && document.getElementById("timersetbox").value > 1) {

        if (motorStatus === "garageOpened") {
            var time = document.getElementById("timersetbox").value + "";
            if(disabled === false) {
                console.log(time);
                particle.callFunction({ deviceId: deviceId, name: 'afterTimer', argument: time, auth: token }).then(callSuccess, callFailure);
            } else {
                disable = false;
                alert("You stopped the timer by clicking the disable button!");
            }

            //toClosed();
        }

    } else {
        alert("Sorry, we do not support any time over 100 seconds and under 2 seconds");
    }

}

// STUDIO 7
var particle = new Particle();
var token;
var temp;

var login = 'scotthom@wustl.edu'
var password = 'everettscott'
var deviceId = '430020001951353337343731'  // Comes from the number in the particle.io Console


// Call back function for login success/failure
function loginSuccess(data) {
    console.log('API call completed on promise resolve: ', data.body.access_token);
    token = data.body.access_token;
}

function loginError(error) {
    console.log('API call completed on promise fail: ', error);
}

// Try to login
particle.login({username: login, password: password}).then(loginSuccess, loginError)

function callSuccess(data) {
    console.log('Function called succesfully:', data);
}

function callFailure(error) {
    console.log('An error occurred:', error);
}


document.getElementById('submit-button').addEventListener('click', runLogin);

function runLogin() {
    toControl();
    getStatus();
    writeStatus();
}

//this is where the js will read from the particle (should direct work flow)

var motorStatus; // everything should be premised off this variable
var fromParticle;


function writeStatus() {
    particle.callFunction({ deviceId: deviceId, name: 'writeStatus', argument: 'writeStatus', auth: token }).then(callSuccess, callFailure);
}

function getStatus() {
    particle.getEventStream({ deviceId: deviceId, auth: token }).then(function(stream) {
        stream.on('event', function(data) {
            motorStatus = data.data;
            console.log(motorStatus);
            doorControl();
        });
    });
}

//below are helper functions that call the particle functions

function toClosing() {
    particle.callFunction({ deviceId: deviceId, name: 'toClosing', argument: 'toClosing', auth: token }).then(callSuccess, callFailure);
}

function toClosingAfterTimer() {
    particle.callFunction({ deviceId: deviceId, name: 'toClosingAfterTimer', argument: 'toClosingAfterTimer', auth: token }).then(callSuccess, callFailure);
}

function toOpening() {
    particle.callFunction({ deviceId: deviceId, name: 'toOpening', argument: 'toOpening', auth: token }).then(callSuccess, callFailure);
}

function toOpened() {
    particle.callFunction({ deviceId: deviceId, name: 'toOpened', argument: 'toOpened', auth: token }).then(callSuccess, callFailure);
}

function toClosed() {
    particle.callFunction({ deviceId: deviceId, name: 'toClosed', argument: 'toClosed', auth: token }).then(callSuccess, callFailure);
}

function toFault() {
    particle.callFunction({ deviceId: deviceId, name: 'toFault', argument: 'toFault', auth: token }).then(callSuccess, callFailure);
}


document.getElementById("open-control").addEventListener('click', checkOpening); // goes to open
document.getElementById("close-control").addEventListener('click', checkClosing); // goes to closed
document.getElementById("door-control").addEventListener('click', updateMasterButton); // switches from opening to closing from opened/ closed
document.getElementById("fault-control").addEventListener('click', checkFault); // switches from opening to closing from opened/ closed


// below we are checking the current status

function checkOpening() {
    if(motorStatus == "motorOpening") {
        document.getElementById("door-control").value = "Close";
        document.getElementById("indicator").innerHTML = "Status: Opened";
        openedPicture();
        toOpened();
    }
}

function checkClosing() {
    if(motorStatus == "motorClosing") {
        document.getElementById("door-control").value = "Open";
        document.getElementById("indicator").innerHTML = "Status: Closed";
        closedPicture();
        toClosed();
    }
}


function updateMasterButton() {
    if(motorStatus == "garageOpened") {
        document.getElementById("door-control").value = "Closing";
        document.getElementById("indicator").innerHTML = "Status: Closing";
        changingPicture();
        toClosing();
    } else if(motorStatus == "garageClosed") {
        document.getElementById("door-control").value = "Opening";
        document.getElementById("indicator").innerHTML = "Status: Opening";
        changingPicture();
        toOpening();
    } else if(motorStatus === "faultOpen") {
        document.getElementById("door-control").value = "Closing";
        document.getElementById("indicator").innerHTML = "Status: Closing";
        changingPicture();
        toClosing();
    } else if(motorStatus === "faultClose") {
        document.getElementById("door-control").value = "Opening";
        document.getElementById("indicator").innerHTML = "Status: Opening";
        changingPicture();
        toOpening();    }
    }

    function checkFault() {
        if(motorStatus == "motorOpening" || motorStatus == "motorClosing") {
            toFault();
        }
    }


    //below is updating the web page based on particle output.... all is fine

    function doorControl() { // particle to javascript functions below (these are functions correctly)

        if (motorStatus === "motorOpening") {

            document.getElementById("door-control").value = "Opening";
            document.getElementById("indicator").innerHTML = "Status: Opening";
            changingPicture();

        } else if(motorStatus === "motorClosing") {

            document.getElementById("door-control").value = "Closing";
            document.getElementById("indicator").innerHTML = "Status: Closing";
            changingPicture();

        } else if(motorStatus === "garageOpened") {

            document.getElementById("door-control").value = "Close";
            document.getElementById("indicator").innerHTML = "Status: Opened";
            openedPicture();

        } else if(motorStatus === "garageClosed") {

            document.getElementById("door-control").value = "Open";
            document.getElementById("indicator").innerHTML = "Status: Closed";
            closedPicture();

        } else if(motorStatus === "faultOpen") {

            document.getElementById("door-control").value = "Close";
            document.getElementById("indicator").innerHTML = "Status: Fault Opening";
            changingPicture();

        } else if(motorStatus === "faultClose") {

            document.getElementById("door-control").value = "Open";
            document.getElementById("indicator").innerHTML = "Status: Fault Closing";
            changingPicture();

        }
    }

    function closedPicture() {
        closedImage.hidden = false;
        openedImage.hidden = true;
        changingImage.hidden = true;
    }

    function openedPicture() {
        closedImage.hidden = true;
        openedImage.hidden = false;
        changingImage.hidden = true;
    }

    function changingPicture() {
        closedImage.hidden = true;
        openedImage.hidden = true;
        changingImage.hidden = false;
    }
