<?php
if (isset($_GET["getStatus"])) {
  $fp = fopen("log.txt", "a");
  fprintf($fp, "%s: de status wordt opgevraagd\r\n", date("H:i:s"));
  fclose($fp);
  if (!file_exists("db.txt")) {
    echo "0";
  } else {
    $fp = fopen("db.txt", "r");
    fpassthru($fp);  //heel de file doorgeven
    fclose($fp);
  }
  exit;
}

if (!empty($_POST)) {
  if (empty($_POST['status'])) {
    $_GET["setNumber"] = "0";
  }
  $fp = fopen("log.txt", "a");
  fprintf($fp, "%s: de volgende status werd ingegeven: '%d'\r\n", date("H:i:s"), $_POST['status']);
  fclose($fp);
  $fp = fopen("db.txt", "w");
  fwrite($fp, $_POST['status'] . "\r\n");
  fclose($fp);
}

?>
<!DOCTYPE html>
<html>

<head>
  <script>
    function setNumber() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var numberInput = document.querySelector('.status');
          numberInput.value = this.responseText;
        }
      };
      var data = document.querySelector('.status').value;
      console.log(data);
      xhttp.open("GET", "index.php?setNumber=" + data, true);
      xhttp.send();
    }

    function getNumber() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var numberInput = document.querySelector('.status');
          numberInput.value = this.responseText;
          setTimeout(getNumber, 1000);
        }
      };
      xhttp.open("GET", "index.php?getStatus", true);
      xhttp.send();
    }
    setTimeout(getNumber, 10);
  </script>
</head>

<body>
<form action="" method="post">
    <input type="hidden" class="status" name="status" value="empty">
</form>

<form action="" method="post">
    <input type="hidden" class="status" name="status" value="ochtendlicht">
    <input type="submit" value="ochtendlicht">
</form>

  <form action="" method="post">
    <input type="hidden" class="status" name="status" value="nachtlicht">
    <input type="submit" value="nachtlicht">
  </form>

    <form action="" method="post">
    <input type="hidden" class="status" name="status" value="daglicht">
    <input type="submit" value="daglicht">
  </form>



</body>

</html>