/**
 * service.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 *Ankit Gupta
 *ankitgupta@college.harvard.edu
 *
 * Implements a shuttle service.
 *
 * Additional Features: Speed up/Speed down (r/e keys), 
 * color coded seat map, point tracking (in announcements)
 *
 */
// default height
var HEIGHT = 0.8;

// default latitude
var LATITUDE = 42.3745615030193;

// default longitude
var LONGITUDE = -71.11803936751632;

// default heading
var HEADING = 1.757197490907891;

// default number of seats
var SEATS = 10;

// default velocity
var VELOCITY = 50;

// global reference to shuttle's marker on 2D map
var bus = null;

// global reference to 3D Earth
var earth = null;

// global reference to 2D map
var map = null;

// global reference to shuttle
var shuttle = null;

// global score
var score = 0;

// load version 1 of the Google Earth API
google.load("earth", "1");

// load version 3 of the Google Maps API
google.load("maps", "3", {other_params: "sensor=false"});

// once the window has loaded
$(window).load(function() {

    // listen for keydown anywhere in body
    $(document.body).keydown(function(event) {
        return keystroke(event, true);
    });

    // listen for keyup anywhere in body
    $(document.body).keyup(function(event) {
        return keystroke(event, false);
    });

    // listen for click on Drop Off button
    $("#dropoff").click(function(event) {
        dropoff();
    });

    // listen for click on Pick Up button
    $("#pickup").click(function(event) {
        pickup();
    });

    // load application
    load();
});

// unload application
$(window).unload(function() {
    unload();
});

/**
* Renders seating chart.
*/
function chart()
{
    var html = "<ol start='0'>";
    for (var i = 0; i < shuttle.seats.length; i++)
    {
        if (shuttle.seats[i] == null)
        {
            html += "<li>Empty Seat</li>";
        }
        else
        {
            html += "<li>" + shuttle.seats[i].name + ", " + shuttle.seats[i].house + "</li>";
        }
    }
    html += "</ol>";
    $("#chart").html(html);
}

/**
* Sorts the seating according to house in alphabetical order
*/
function sortseating() {
var old_seats = shuttle.seats;
var new_seats = new Array();

// sort the old seating according to alphabetical order
old_seats.sort(function(a, b) {
if (a != null && b != null) {
var houseA = a.house.toUpperCase();
     var houseB = b.house.toUpperCase();
     return (houseA < houseB) ? 1 : (houseA > houseB) ? -1 : 0;
     }
    
     // if one or both of the values are empty, report -1/1/0 as seems fit
     else if (b == null && a != null) {
     return 1;
     } else if (a == null && b != null) {
     return -1;
     } else {
     return 0;
     }
});
for (var i in old_seats) {
new_seats.unshift(old_seats[i]);
}
shuttle.seats = new_seats;
}


/**
* Drops up passengers if their stop is nearby.
*/
function dropoff()
{
    var alone = true;
    for (var i in shuttle.seats) {
     if (shuttle.seats[i] != null) {
     var lat = HOUSES[shuttle.seats[i].house].lat;
     var lng = HOUSES[shuttle.seats[i].house].lng;
     if (shuttle.distance(lat, lng) < 30.0) {
     alone = false;
     shuttle.seats[i] = null;
     score++;
     $("#score").html("Your score: " + score);
     sortseating();
     chart();
     }
     }
    }
    if (alone) {
     score--;
     $("#score").html("Your score: " + score);
     $("#announcements").html("No destinations in the neighbourhood.");
    }
}

/**
* Called if Google Earth fails to load.
*/
function failureCB(errorCode)
{
    // report error unless plugin simply isn't installed
    if (errorCode != ERR_CREATE_PLUGIN)
    {
        alert(errorCode);
    }
}

/**
* Handler for Earth's frameend event.
*/
function frameend()
{
    shuttle.update();
}

/**
* Called once Google Earth has loaded.
*/
function initCB(instance)
{
    // retain reference to GEPlugin instance
    earth = instance;

    // specify the speed at which the camera moves
    earth.getOptions().setFlyToSpeed(100);

    // show buildings
    earth.getLayerRoot().enableLayerById(earth.LAYER_BUILDINGS, true);

    // disable terrain (so that Earth is flat)
    earth.getLayerRoot().enableLayerById(earth.LAYER_TERRAIN, false);

    // prevent mouse navigation in the plugin
    earth.getOptions().setMouseNavigationEnabled(false);

    // instantiate shuttle
    shuttle = new Shuttle({
        heading: HEADING,
        height: HEIGHT,
        latitude: LATITUDE,
        longitude: LONGITUDE,
        planet: earth,
        seats: SEATS,
        velocity: VELOCITY
    });

    // synchronize camera with Earth
    google.earth.addEventListener(earth, "frameend", frameend);

    // synchronize map with Earth
    google.earth.addEventListener(earth.getView(), "viewchange", viewchange);

    // update shuttle's camera
    shuttle.updateCamera();

    // show Earth
    earth.getWindow().setVisibility(true);

    // render seating chart
    chart();

    // populate Earth with passengers and houses
    populate();
}

/**
* Handles keystrokes.
*/
function keystroke(event, state)
{	
$("#announcements").html("yeye coolcool");
    // ensure we have event
    if (!event)
    {
        event = window.event;
        
    }

    // left arrow
    if (event.keyCode == 37)
    {
        shuttle.states.turningLeftward = state;
        return false;
    }

    // up arrow
    else if (event.keyCode == 38)
    {
        shuttle.states.tiltingUpward = state;
        return false;
    }

    // right arrow
    else if (event.keyCode == 39)
    {
        shuttle.states.turningRightward = state;
        return false;
    }

    // down arrow
    else if (event.keyCode == 40)
    {
        shuttle.states.tiltingDownward = state;
        return false;
    }

    // A, a
    else if (event.keyCode == 65 || event.keyCode == 97)
    {
        shuttle.states.slidingLeftward = state;
        return false;
    }

    // D, d
    else if (event.keyCode == 68 || event.keyCode == 100)
    {
        shuttle.states.slidingRightward = state;
        return false;
    }
  
    // S, s
    else if (event.keyCode == 83 || event.keyCode == 115)
    {
        shuttle.states.movingBackward = state;
        return false;
    }

    // W, w
    else if (event.keyCode == 87 || event.keyCode == 119)
    {
        shuttle.states.movingForward = state;
        return false;
    }
    
    // r (for speeding up)
    else if (event.keyCode == 82) {
     shuttle.velocity++;
    }
    
    // e (for resetting the velocity)
    else if (event.keyCode == 69) {
     shuttle.velocity = config.velocity;
    }
    
    // q (for slowing down)
    else if (event.keyCode == 81) {
     if (shuttle.velocity > 0) {
     shuttle.velocity--;
     }
    }
  
    return true;
}

/**
* Loads application.
*/
function load()
{
    // embed 2D map in DOM
    var latlng = new google.maps.LatLng(LATITUDE, LONGITUDE);
    map = new google.maps.Map($("#map").get(0), {
        center: latlng,
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        scrollwheel: false,
        zoom: 17,
        zoomControl: true
    });

    // prepare shuttle's icon for map
    bus = new google.maps.Marker({
        icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/bus.png",
        map: map,
        title: "you are here"
    });

    // embed 3D Earth in DOM
    google.earth.createInstance("earth", initCB, failureCB);
}

/**
* Checks whether a person is a freshman (if its house is in HOUSES)
*/
function isFreshman(passenger) {
if (HOUSES[passenger.house] == null) {
return true;
} else {
return false;
}
}

/**
* Returns the index of the first free seat in the shuttle, else returns -1
*/
function emptyseats() {
for (var i in shuttle.seats) {
if (shuttle.seats[i] == null) {
return i;
}
}
return -1;
}

/**
* Picks up nearby passengers.
*/
function pickup()
{	
// if shuttle is alone within radius 15.0, alone = true, else false
var alone = true;
    for (var i in PASSENGERS) {
     if (PASSENGERS[i].placemark != null) {
var lat = PASSENGERS[i].placemark.getGeometry().getLatitude();
var lng = PASSENGERS[i].placemark.getGeometry().getLongitude();

if (shuttle.distance(lat, lng) < 15.0 && emptyseats() >= 0 && !isFreshman(PASSENGERS[i])) {
alone = false;
shuttle.seats[emptyseats()] = PASSENGERS[i];
var features = earth.getFeatures();
features.removeChild(PASSENGERS[i].placemark);
PASSENGERS[i].marker.setMap(null);
PASSENGERS[i].placemark = null;
sortseating();
chart();
}
}
    }
    if (emptyseats() == -1) {
$("#announcements").html("The shuttle bus is full.");
}
    else if (alone) {
     $("#announcements").html("There is no one in the neighbourhood of the shuttle.");
    }
}

/**
* Populates Earth with passengers and houses.
*/
function populate()
{
    // mark houses
    for (var house in HOUSES)
    {
        // plant house on map
        new google.maps.Marker({
            icon: "https://google-maps-icons.googlecode.com/files/home.png",
            map: map,
            position: new google.maps.LatLng(HOUSES[house].lat, HOUSES[house].lng),
            title: house
        });
    }

    // get current URL, sans any filename
    var url = window.location.href.substring(0, (window.location.href.lastIndexOf("/")) + 1);

    // scatter passengers
    for (var i = 0; i < PASSENGERS.length; i++)
    {
        // pick a random building
        var building = BUILDINGS[Math.floor(Math.random() * BUILDINGS.length)];

        // prepare placemark
        var placemark = earth.createPlacemark("");
        placemark.setName(PASSENGERS[i].name + " to " + PASSENGERS[i].house);

        // prepare icon
        var icon = earth.createIcon("");
        icon.setHref(url + "/img/" + PASSENGERS[i].username + ".jpg");

        // prepare style
        var style = earth.createStyle("");
        style.getIconStyle().setIcon(icon);
        style.getIconStyle().setScale(4.0);

        // prepare stylemap
        var styleMap = earth.createStyleMap("");
        styleMap.setNormalStyle(style);
        styleMap.setHighlightStyle(style);

        // associate stylemap with placemark
        placemark.setStyleSelector(styleMap);

        // prepare point
        var point = earth.createPoint("");
        point.setAltitudeMode(earth.ALTITUDE_RELATIVE_TO_GROUND);
        point.setLatitude(building.lat);
        point.setLongitude(building.lng);
        point.setAltitude(0.0);

        // associate placemark with point
        placemark.setGeometry(point);

        // add placemark to Earth
        earth.getFeatures().appendChild(placemark);

        // add marker to map
        var marker = new google.maps.Marker({
            icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/man.png",
            map: map,
            position: new google.maps.LatLng(building.lat, building.lng),
            title: PASSENGERS[i].name + " at " + building.name
        });

// add placemark and marker as fields in PASSENGER associative array
        PASSENGERS[i].placemark = placemark;
        PASSENGERS[i].marker = marker;
    }
}

/**
* Handler for Earth's viewchange event.
*/
function viewchange()
{
    // keep map centered on shuttle's marker
    var latlng = new google.maps.LatLng(shuttle.position.latitude, shuttle.position.longitude);
    map.setCenter(latlng);
    bus.setPosition(latlng);
}

/**
* Unloads Earth.
*/
function unload()
{
    google.earth.removeEventListener(earth.getView(), "viewchange", viewchange);
    google.earth.removeEventListener(earth, "frameend", frameend);
}
