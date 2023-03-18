const char html [] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <link rel="icon" href="https://png.pngtree.com/png-clipart/20230120/ourmid/pngtree-cute-chicken-png-image_6563222.png">
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
  <meta name="author" content="AltoDev" />
  <link href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.4.1/font/bootstrap-icons.css" rel="stylesheet" />
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css"
    integrity="sha384-GJzZqFGwb1QTTN6wy59ffF1BuGJpLSa9DkKMp0DgiMDm4iYMj70gZWKYbI706tWS"
    crossorigin="anonymous">
  <title>Smart Kurnik</title>
  <style>
    .feature {
      display: inline-flex;
      align-items: center;
      justify-content: center;
      height: 4rem;
      width: 4rem;
      font-size: 2rem;
    }
    .rounded-3 {
      border-radius: 0.3rem !important;
    }
    h5, .h5, h2, .h2 {
      margin-top: 0;
      margin-bottom: 0.5rem;
      font-weight: 500;
      line-height: 1.2;
    }

    h2, .h2 {
      font-size: calc(1.325rem + 0.9vw);
    }
    @media (min-width: 1200px) {
      h2, .h2 {
        font-size: 2rem;
      }
    }
    h5, .h5 {
      font-size: 1.25rem;
    }
  </style>
</head>
<body>
  <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
      <div class="container px-lg-5">
          <a class="navbar-brand" href="#!">
            <img style="height: 80px;" src="https://png.pngtree.com/png-clipart/20230120/ourmid/pngtree-cute-chicken-png-image_6563222.png" alt="logo" border="0">
            SMART KURNIK
          </a>
          <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation"><span class="navbar-toggler-icon"></span></button>
          <div class="collapse navbar-collapse" id="navbarSupportedContent">
              <ul class="navbar-nav ms-auto my-2 my-lg-0">
                  <li class="nav-item"><a class="nav-link active" aria-current="page">Status</a></li>
                  <li class="nav-item"><a class="nav-link" id="openHoursModal">Zaplanuj pracę kurnika</a></li>
                  <li class="nav-item"><a class="nav-link" id="openManualActionsModal">Wywołaj ręcznie akcję</a></li>
              </ul>
          </div>
      </div>
  </nav>
  <header class="py-3">
    <div class="container-fluid">
      <div class="row">
        <div class="col-md-12">
          <div id="alert" class="alert alert-success fade in text-center" role="alert">
            <span id="alertText"></span>
          </div>
        </div>
      </div>
    </div>
  </header>
  <section class="pt-4">
      <div class="container px-lg-5">
          <div class="row gx-lg-5">
              <div class="col-lg-6 col-xxl-4 mb-5">
                  <div class="card bg-light border-0 h-100">
                      <div class="card-body text-center p-4 p-lg-5">
                          <div id="switchKurnikIcon" class="feature bg-light bg-gradient text-white rounded-3"><i class="bi bi-house"></i></div>
                          <h2 class="fs-4 pt-3 pb-3 fw-bold">Automatyzacja</h2>
                          <button id="switchKurnik" type="button" class="btn btn-danger">
                            <span id="switchKurnikButtonText">Ładowanie...</span>
                          </button>
                      </div>
                  </div>
              </div>
              <div class="col-lg-6 col-xxl-4 mb-5">
                  <div class="card bg-light border-0 h-100">
                      <div class="card-body text-center p-4 p-lg-5">
                          <div id="doorIcon" class="feature bg-light bg-gradient text-white rounded-3"><i class="bi bi-door-closed"></i></div>
                          <h2 class="fs-4 pt-3 fw-bold">Drzwiczki</h2>
                          <p id="doorText" class="mb-0">Ładowanie...</p>
                      </div>
                  </div>
              </div>
              <div class="col-lg-6 col-xxl-4 mb-5">
                  <div class="card bg-light border-0 h-100">
                      <div class="card-body text-center p-4 p-lg-5">
                          <div id="dayLightIcon" class="feature bg-light bg-gradient text-white rounded-3"><i class="bi bi-lightbulb-fill"></i></div>
                          <h2 class="fs-4 pt-3 fw-bold">Światło dzienne</h2>
                          <p id="dayLightText" class="mb-0">Ładowanie...</p>
                      </div>
                  </div>
              </div>
              <div class="col-lg-6 col-xxl-4 mb-5">
                  <div class="card bg-light border-0 h-100">
                      <div class="card-body text-center p-4 p-lg-5">
                          <div id="nightLightIcon" class="feature bg-light bg-gradient text-white rounded-3"><i class="bi bi-lightbulb"></i></div>
                          <h2 class="fs-4 pt-3 fw-bold">Światło nocne</h2>
                          <p id="nightLightText" class="mb-0">Ładowanie...</p>
                      </div>
                  </div>
              </div>
          </div>
      </div>
  </section>
  <!-- Footer-->
  <footer class="py-5 bg-dark">
      <div class="container"><p class="m-0 text-center text-white">Copyright &copy; AltoDev 2023</p></div>
  </footer>

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
      </div>
    </div>
  </div>
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
      const iconBackgroundClassPrefix = "feature bg-gradient text-white rounded-3";
      $('#switchKurnikButtonText').text(response.coornickTurnedOn ? "WYŁĄCZ" : "WŁĄCZ"); 
      $('#switchKurnikIcon').prop( "class", `${iconBackgroundClassPrefix} ${response.coornickTurnedOn ? "bg-success" : "bg-secondary"}`);
      kurnikTurnedOn = response.coornickTurnedOn;
      $('#dayLightText').text(response.dayLights ? "Włączone" : "Wyłączone");
      $('#nightLightText').text(response.nightLights ? "Włączone" : "Wyłączone"); 
      $('#doorText').text(response.isDoorOpening ? "Otwieranie..." 
          : response.isDoorClosing ? "Zamykanie..." 
          : response.isDoorOpened ? "Otwarte"
          : response.isDoorClosed ? "Zamknięte" : "Błąd");
      if (response.lastActionManual) {
        $('#doorIcon').prop( "class", `${iconBackgroundClassPrefix} ${response.isDoorClosed || response.isDoorOpened 
          ? "bg-success" : response.isDoorOpening || response.isDoorClosing ? "bg-secondary" : "bg-danger"}`);
        $('#dayLightIcon').prop( "class", `${iconBackgroundClassPrefix} ${response.dayLights ? "bg-success" : "bg-secondary"}`);
        $('#nightLightIcon').prop( "class", `${iconBackgroundClassPrefix} ${response.nightLights ? "bg-success" : "bg-secondary"}`);
      } else {
        $('#doorIcon').prop( "class", `${iconBackgroundClassPrefix} ${(shouldDoorBeOpen(response.lastAction) && response.isDoorOpened)
          || (shouldDoorBeClosed(response.lastAction) && response.isDoorClosed)? "bg-success" : "bg-danger"}`);
        const dayLightsOk = areDayLightsAsScheduled(response.dayLights, response.lastAction);
        $('#dayLightIcon').prop( "class", `${iconBackgroundClassPrefix} ${ dayLightsOk && response.dayLights ? "bg-success" 
          : dayLightsOk && !response.dayLights ? "bg-secondary" : "bg-danger"}`);
        const nightLightsOk = areNightLightsAsScheduled(response.nightLights, response.lastAction);
        $('#nightLightIcon').prop( "class", `${iconBackgroundClassPrefix} ${ nightLightsOk && response.nightLights ? "bg-success" 
          : nightLightsOk && !response.nightLights ? "bg-secondary" : "bg-danger"}`);
      }
    }
    function shouldDoorBeOpen(lastAction) {
      return lastAction === 1 || lastAction === 2;
    }
    function shouldDoorBeClosed (lastAction) {
      return lastAction === 3 || lastAction === 4;
    }
    function areDayLightsAsScheduled (dayLights, lastAction) {
      return dayLights ? lastAction === 1 : lastAction > 1;
    }
    function areNightLightsAsScheduled(nightLights, lastAction) {
      if (nightLights) {
        return lastAction === 2 || lastAction === 3;
      }
      return lastAction === 1 || lastAction === 4;
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
      $('#alert').prop( "class", "alert alert-success fade in text-center show");
      $('#alert').alert();
      setTimeout(hideToast, 5000);
    }
    function showErrorToast(msg, err) {
      if (err) console.error(err);
      $('#alertText').text(msg); 
      $('#alert').prop( "class", "alert alert-danger fade in text-center show");
      $('#alert').alert();
      setTimeout(hideToast, 5000);
    }
    function hideToast() {
      $("#alert").prop( "class", "hide");
      $("#alertText").text(null);
    }

    $("#openHoursModal").on("click", function(){
      $('#scheduleActionsModal').modal('show');
      get("getActionTimes", parseActionTimes);
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
    });

    $("#openManualActionsModal").on("click", function(){
      $('#manualActionsModal').modal('show');
      $("#lightsDayDoorOpen").button().click(function(){
          get("lightsDayDoorOpen", () => showSuccessToast("Akcja wykonana poprawnie"), (err) => showErrorToast("Wystąpił błąd!", err));
          $('#manualActionsModal').modal('hide');
      }); 
      $("#lightsNight").button().click(function(){
          get("lightsNight", () => showSuccessToast("Akcja wykonana poprawnie"), (err) => showErrorToast("Wystąpił błąd!", err));
          $('#manualActionsModal').modal('hide');
      }); 
      $("#doorClosed").button().click(function(){
          get("doorClosed", () => showSuccessToast("Akcja wykonana poprawnie"), (err) => showErrorToast("Wystąpił błąd!", err));
          $('#manualActionsModal').modal('hide');
      }); 
      $("#lightsOff").button().click(function(){
          get("lightsOff", () => showSuccessToast("Akcja wykonana poprawnie"), (err) => showErrorToast("Wystąpił błąd!", err));
          $('#manualActionsModal').modal('hide');
      }); 
    }); 

    $("#switchKurnik").button().click(function(){
        put("coornickSwitch", { previousState: kurnikTurnedOn },
          () => showSuccessToast("Akcja wykonana poprawnie"),
          (err) => showErrorToast("Wystąpił błąd!", err)
        )
    });
    get("status", parseStatus);
    setInterval(async () => { get("status", parseStatus);}, 5000);
    $("#alert").prop("class", "hide");
  </script>
</body>
</html>
)=====";