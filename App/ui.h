const char html [] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <link rel="stylesheet" href=
"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css"
    integrity=
"sha384-GJzZqFGwb1QTTN6wy59ffF1BuGJpLSa9DkKMp0DgiMDm4iYMj70gZWKYbI706tWS"
    crossorigin="anonymous">
  <link href="https://fonts.googleapis.com/icon?family=Material+Icons"
    rel="stylesheet">
  <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@48,400,0,0" />
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Kurnik Państwa Nowaków</title>
  <style>
    h1 {
      color: grey;
      text-align: center;
    }
    div.one {
      margin-top: 40px;
      text-align: center;
    }
    button {
      margin-top: 10px;
    }
    .customRow {
      margin-top: 20px;
      margin-bottom: 20px;
    }
  </style>
</head>
<body>
  <div class="container-fluid">
    <div id="alert" class="alert alert-success" role="alert">
      <p id="alertText"></p>
    </div>
    <!-- TODO: Add logo -->
    <div class="one">
      <div class="row customRow">
        <div class="col text-center">
          <button id="openHoursModal" type="button" class="btn btn-light">
            Zaplanuj pracę kurnika
          </button>
        </div>
      </div>
      <div class="row customRow">
        <div class="col text-center">
          <div class="custom-control custom-switch">
            <label class="custom-control-label text-right" for="dayLightSwitch">
              <span class="material-symbols-outlined">light_mode</span> Światło dzienne
            </label>
            <input type="checkbox" class="custom-control-input" id="dayLightSwitch" disabled>
          </div>
        </div>
      </div>
      <div class="row customRow">
        <div class="col text-center">
          <div class="custom-control custom-switch">
            <label class="custom-control-label" for="nightLightSwitch">
              <span class="material-symbols-outlined">nightlight</span> Światło nocne
            </label>
            <input type="checkbox" class="custom-control-input" id="nightLightSwitch" disabled>
          </div>
        </div>
      </div>
      <!-- TODO: Add main switch -->
      <div class="row customRow">
        <div class="col text-center">
          <button id="switchKurnik" type="button" class="btn btn-light">
            <p id="switchKurnikText"></p>
          </button>
        </div>
      </div>
      <div class="row customRow">
        <div class="col text-center">
          <button id="openManualActionsModal" type="button" class="btn btn-light">
            Wywołaj ręcznie akcję
          </button>
        </div>
      </div>
    </div>
  </div>

  <!-- Modal hours-->
  <div class="modal fade" id="scheduleActionsModal" tabindex="-1" role="dialog" aria-labelledby="modalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title" id="modalLabel">Zaplanuj pracę kurnika</h5>
          <button type="button" class="close" data-dismiss="modal" aria-label="Close">
            <span aria-hidden="true">&times;</span>
          </button>
        </div>
        <div class="modal-body">
          <form>
            <div class="form-group">
              <i>Uwaga: akcje są wykonywane z dokładnością do 15 minut</i>
            </div>
            <div class="form-group">
              <label for="lightsDayDoorOpenTime">Otwieranie drzwiczek i zapalanie światła dziennego</label>
              <input type="time" step="900" class="form-control" id="lightsDayDoorOpenTime">
            </div>
            <div class="form-group">
              <label for="lightsNightTime">Zmiana świateł na nocne</label>
              <input type="time" step="900" class="form-control" id="lightsNightTime">
            </div>
            <div class="form-group">
              <label for="doorClosedTime">Zamykanie drzwiczek</label>
              <input type="time" step="900" class="form-control" id="doorClosedTime">
            </div>
            <div class="form-group">
              <label for="lightsOffTime">Gaszenie świateł</label>
              <input type="time" step="900" class="form-control" id="lightsOffTime">
            </div>
          </form>
        </div>
        <div class="modal-footer">
          <button type="button" class="btn btn-light" data-dismiss="modal">Anuluj</button>
          <button id="scheduleActions" type="submit" class="btn btn-dark">Zapisz</button>
        </div>
      </div>
    </div>
  </div>

  <!-- Modal manual actions -->
  <div class="modal fade" id="manualActionsModal" tabindex="-1" role="dialog" aria-labelledby="modalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title" id="modalLabel">Wywołaj ręcznie akcję</h5>
          <button type="button" class="close" data-dismiss="modal" aria-label="Close">
            <span aria-hidden="true">&times;</span>
          </button>
        </div>
        <div class="modal-body">
          <form>
            <div class="form-group">
              <button id="lightsDayDoorOpen" type="button" class="btn btn-light">Otwórz drzwiczki i zapal światło dzienne</button>
            </div>
            <div class="form-group">
              <button id="lightsNight" type="button" class="btn btn-light">Zmień światło na nocne</button>
            </div>
            <div class="form-group">
              <button id="doorClosed" type="button" class="btn btn-light">Zamknij drzwiczki</button>
            </div>
            <div class="form-group">
              <button id="lightsOff" type="button" class="btn btn-light">Zgaś światła</button>
            </div>
          </form>
        </div>
        <div class="modal-footer">
          <button type="button" class="btn btn-light" data-dismiss="modal">Zamknij</button>
        </div>
      </div>
    </div>
  </div>

  <!-- Optional JavaScript -->
  <!-- jQuery first, then Popper.js, then Bootstrap JS -->
  
  <script src=
"https://code.jquery.com/jquery-3.3.1.slim.min.js"
    integrity=
"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo"
    crossorigin="anonymous"></script>
  <script src=
"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.6/umd/popper.min.js"
    integrity=
"sha384-wHAiFfRlMFy6i5SRaxvfOCifBUQy1xHdJ/yoi7FRNXMRBu5WHdZYu1hA6ZOblgut"
    crossorigin="anonymous"></script>
  <script src=
"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js"
    integrity=
"sha384-B0UglyR+jN6CkvvICOB2joaf5I4l3gm9GU6Hc1og6Ls7i6U/mkkaduKaBhlAXv9k"
    crossorigin="anonymous"></script>
  <script type="text/javascript">
    let kurnikTurnedOn = 1;
    function parseStatus(responseText) {
      const response = JSON.parse(responseText);
      console.log(response);
      // TODO: add door status
      $('#dayLightSwitch').prop( "checked", response.dayLights);
      $('#nightLightSwitch').prop( "checked", response.nightLights);
      $('#switchKurnikText').text(response.coornickTurnedOn ? "Wyłącz kurnik" : "Włącz kurnik"); 
      kurnikTurnedOn = response.coornickTurnedOn;
    }
    function periodToTime(period) {
      const hours = `${Math.floor(period / 4)}`.padStart(2, '0');
      const minutes = `${period % 4 * 15}`.padStart(2, '0');
      return `${hours}:${minutes}`;
    }
    function parseActionTimes(responseText) {
      const actionTimes = JSON.parse(responseText);
      console.log(actionTimes);
      $('#lightsDayDoorOpenTime').prop("value", periodToTime(actionTimes.lightsDayDoorOpen));
      $('#lightsNightTime').prop("value", periodToTime(actionTimes.lightsNight));
      $('#doorClosedTime').prop("value", periodToTime(actionTimes.doorClosed));
      $('#lightsOffTime').prop("value", periodToTime(actionTimes.lightsOff));
    }
    function onLoad(req, successCallback, errorCallback) {
      if(req.status == 200) {
        successCallback(req.responseText);
      } else {
        errorCallback(req.response);
      }
    }
    function get(url, successCallback, errorCallback){
      const req = new XMLHttpRequest();
      if (successCallback || errorCallback) req.addEventListener("load", () => onLoad(req, successCallback, errorCallback));
      if (errorCallback) req.addEventListener("error", errorCallback);
      req.open("GET", window.location.href + url);
      req.send();
    }
    function put(url, data, successCallback, errorCallback) {
      const req = new XMLHttpRequest();
      req.open('PUT', window.location.href + url, true);
      req.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
      if (successCallback || errorCallback) req.addEventListener("load", () => onLoad(req, successCallback, errorCallback));
      if (errorCallback) req.addEventListener("error", errorCallback);
      const dataToSend = Object.keys(data).reduce(function(arr, key) {
        return [ ...arr, `${key}=${data[key]}`];
      }, []).join("&");
      req.send(dataToSend);
    }
    function timeToPeriod(timeString) {
      if (!timeString) return null;
      const [hours, minutes] = timeString.split(':');
      const period = hours * 4 + (minutes / 15);
      return period;
    }
    function hoursOk(hours) {
      return hours 
      && hours.lightsDayDoorOpen < hours.lightsNight 
      && hours.lightsNight < hours.doorClosed 
      && hours.doorClosed < hours.lightsOff;
    }
    function showSuccessToast(msg) {
      $('#alertText').text(msg); 
      $('#alert').prop( "class", "alert alert-success show");
      $('#alert').alert();
      setTimeout(hideToast, 5000);
    }
    function showErrorToast(msg, err) {
      if (err) console.error(err);
      $('#alertText').text(msg); 
      $('#alert').prop( "class", "alert alert-danger show");
      $('#alert').alert();
      setTimeout(hideToast, 5000);
    }
    function hideToast() {
      $("#alert").prop( "class", "hide");
      $("#alertText").text(null);
    }
    $("#lightsDayDoorOpen").button().click(function(){
        get("lightsDayDoorOpens", () => showSuccessToast("Akcja wykonana poprawnie"), (err) => showErrorToast("Wystąpił błąd!", err))
    }); 
    $("#lightsNight").button().click(function(){
        get("lightsNight", () => showSuccessToast("Akcja wykonana poprawnie"), (err) => showErrorToast("Wystąpił błąd!", err))
    }); 
    $("#doorClosed").button().click(function(){
        get("doorClosed", () => showSuccessToast("Akcja wykonana poprawnie"), (err) => showErrorToast("Wystąpił błąd!", err))
    }); 
    $("#lightsOff").button().click(function(){
        get("lightsOff", () => showSuccessToast("Akcja wykonana poprawnie"), (err) => showErrorToast("Wystąpił błąd!", err))
    }); 
    $("#openHoursModal").button().click(function(){
      $('#scheduleActionsModal').modal('show');
      get("getActionTimes", parseActionTimes);
    }); 
    $("#openManualActionsModal").button().click(function(){
      $('#manualActionsModal').modal('show');
    }); 
    $("#scheduleActions").button().click(function(){
        const hours = {
          lightsDayDoorOpen: timeToPeriod(document.getElementById('lightsDayDoorOpenTime').value),
          lightsNight: timeToPeriod(document.getElementById('lightsNightTime').value),
          doorClosed: timeToPeriod(document.getElementById('doorClosedTime').value),
          lightsOff: timeToPeriod(document.getElementById('lightsOffTime').value),
        };
        console.log(hours);
        if (hoursOk(hours)) {
          put("setActionTimes", hours, function() {
            $('#scheduleActionsModal').modal('hide');
            showSuccessToast("Godziny zmienione");
          }, function(err) {
            $('#scheduleActionsModal').modal('hide');
            showErrorToast("Wystąpił błąd podczas zapisu godzin.", err);
          });
        } else {
          showErrorToast("Nieprawidłowe godziny! Popraw je i spróbuj jeszcze raz.");
        }
    });
    $("#switchKurnik").button().click(function(){
        put("coornickSwitch", { previousState: kurnikTurnedOn }, () => showSuccessToast("Akcja wykonana poprawnie"), (err) => showErrorToast("Wystąpił błąd!", err))
    });
    setInterval(async () => { get("status", parseStatus);}, 5000);
    $("#alert").prop("class", "hide");
  </script>
</body>
</html>

)=====";