/**
 * @file
 * The clock block (port of D7 contrib clock 7.x-1.2 clock.js): formats the
 * time in the block's time zone with the PHP date format and, when "update"
 * is on, ticks it every second.
 */
(function (Drupal, drupalSettings, once) {

  Drupal.behaviors.clockDisplay = {
    attach: function (context) {
      var settings = drupalSettings.clock;
      if (!settings || !once('clock', 'html', context).length) {
        return;
      }
      // A PHP date format string.
      var dateFormat = settings.date_format;
      var update = settings.update;
      // The time zone offset in seconds (a string, as D7 passed it).
      var offsetSeconds = String(settings.offset_seconds);
      // Shift the visitor's clock to the target offset.
      var date = new Date();
      if (!settings.local) {
        date = new Date(date.getTime() + (offsetSeconds / 60 + date.getTimezoneOffset()) * 60000);
      }

      var monthNames = [];
      var months = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
      var monthsShort = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
      for (var m = 0; m < 12; m++) {
        monthNames[m + 1] = {'long': Drupal.t(months[m]), 'short': Drupal.t(monthsShort[m])};
      }
      var weekdayNames = [];
      var days = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];
      var daysShort = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'];
      for (var d = 0; d < 7; d++) {
        weekdayNames[d] = {'long': Drupal.t(days[d]), 'short': Drupal.t(daysShort[d])};
      }

      var timezone = [];
      timezone['name'] = settings.time_zone;
      timezone['offsetSeconds'] = offsetSeconds;
      timezone['daylightSavingsTime'] = settings.daylight_savings_time;
      timezone['offsetName'] = settings.offset_name;

      var show = function () {
        var clock = formatDate(date, dateFormat, timezone, monthNames, weekdayNames);
        document.querySelectorAll('div.clock').forEach(function (el) {
          el.textContent = clock;
        });
      };
      show();

      if (update == 1) {
        // Recalculate the date every second.
        window.setInterval(function () {
          date = new Date(date.getTime() + 1000);
          show();
        }, 1000);
      }
    }
  };

})(Drupal, drupalSettings, once);

/**
 * Formats a date into a PHP date string.
 *
 * @param date
 *   A date object.
 * @param dateFormat
 *   A string. See http://php.net/date for possible values.
 * @param timezone
 *   An associative array containing timezone information. It should consist of
 *   following keys:
 *   - timezoneName: The name of the timezone, e.g. 'Europe/London'.
 *   - offsetSeconds: The timezone offset in seconds.
 *   - offsetName: The name of the offset, e.g. 'UTC'.
 *   - daylightSavingsTime: Whether or not the time is in daylight savings time.
 *     '1' if yes, '0' if not.
 * @param monthNames
 *   An array of month names keyed by the numbers 1-12. Each value should in turn
 *   be an array keyed 'long' and 'short' for the long respective the short
 *   month names.
 * @param weekdayNames
 *   An array of weekday names keyed by the numbers 0 (for Sunday) - 6 (for
 *   Saturday). Each value should in turn be an array keyed by 'long' and
 *   'short' for the long respective the short weekday names.
 *
 * @return
 *   A formatted date string.
 */
function formatDate(date, dateFormat, timezone, monthNames, weekdayNames) {
  var timezoneName = timezone['name'];
  var offsetSeconds = timezone['offsetSeconds'];
  var offsetName = timezone['offsetName'];
  var daylightSavingsTime = timezone['daylightSavingsTime'];

  // Initialize the 'formattedDate' variable for later use.
  var formattedDate = '';

  // Prepare variables for the format conversion.

  // Year-related.
  var year = date.getFullYear();
  var yearShort = year % 100;
  // Calculate whether it is a leap year or not. Years that are multiples of 4
  // are leap years, while multiples of 100 are not, but multiples of 400 are.
  var year = date.getFullYear();
  if (((year % 4) == 0) && ((year % 100) != 0) || ((year % 400) == 0)) {
    var leapYear = '1';
  }
  else {
    var leapYear = '0';
  }

  // Month-related.
  var month = date.getMonth();
  // JavaScript starts counting the months with 0 instead of 1.
  month++;
  var monthLeadingZero = ((month < 10) ? '0' + month : month);
  var monthLongName = monthNames[month]['long'];
  var monthShortName = monthNames[month]['short'];
  var day = date.getDate();
  var dayLeadingZero = ((day < 10) ? '0' + day : day);
  switch (day) {
    case 1:
    case 21:
    case 31:
      var dayOfMonthAppend = Drupal.t('st');
      break;
    case 2:
    case 22:
      var dayOfMonthAppend = Drupal.t('nd');
      break;
    case 3:
    case 23:
      var dayOfMonthAppend = Drupal.t('rd');
      break;
    default:
      var dayOfMonthAppend = Drupal.t('th');
      break;
  }
  // Create an array containing month names and lengths.
  var months = new Array();
  months[1] = 31;
  months[2] = ((leapYear == 1) ? 29 : 28);
  months[3] = 31;
  months[4] = 30;
  months[5] = 31;
  months[6] = 30;
  months[7] = 31;
  months[8] = 31;
  months[9] = 30;
  months[10] = 31;
  months[11] = 30;
  months[12] = 31;
  // To calculate the days of the year, iterate through all passed months and then add the current days of the month.
  var daysOfYear = 0;
  for (var m = 1; m < month; m++) {
    daysOfYear += months[m];
  }
  daysOfYear += day;

  // Week-related.
  var weekday = date.getDay();
  if (weekday == 0) {
    var weekNumber = Math.floor((daysOfYear - 7) / 7) + 1;
  }
  else {
    var weekNumber = Math.floor((daysOfYear - weekday) / 7) + 1;
  }
  var weekNumberLeadingZero = ((weekNumber < 10) ? '0' + weekNumber : weekNumber);
  var weekdayLongName = weekdayNames[weekday]['long'];
  var weekdayShortName = weekdayNames[weekday]['short'];

  // Offset-related.
  if (offsetSeconds < 0) {
    var offsetPrefix = '-';
    var offsetHours = Math.floor(parseInt(offsetSeconds.substr(1)) / 3600);
    var offsetHoursLeadingZero = ((offsetHours < 10) ? '0' + offsetHours : offsetHours);
    var offsetMinutes = (parseInt(offsetSeconds.substr(1)) / 60) % 60;
    var offsetMinutesLeadingZero = ((offsetMinutes < 10) ? '0' + offsetMinutes : offsetMinutes);
  }
  else {
    var offsetPrefix = '+';
    var offsetHours = Math.floor(offsetSeconds) / 3600;
    var offsetHoursLeadingZero = ((offsetHours < 10) ? '0' + offsetHours : offsetHours);
    var offsetMinutes = (offsetSeconds / 60) % 60;
    var offsetMinutesLeadingZero = ((offsetMinutes < 10) ? '0' + offsetMinutes : offsetMinutes);
  }

  // Hour-related.
  var hours = date.getHours();
  var hoursLeadingZero = ((hours < 10) ? '0' + hours : hours);
  if (hours == 0) {
    var hoursTwelve = (hours + 12);
  }
  else if (hours > 12) {
    var hoursTwelve = (hours - 12);
  }
  else {
    var hoursTwelve = hours;
  }
  var hoursTwelveLeadingZero = ((hoursTwelve < 10) ? '0' + hoursTwelve : hoursTwelve);

  // Minute-related.
  var minutes = date.getMinutes();
  var minutesLeadingZero = ((minutes < 10) ? '0' + minutes : minutes);

  // Second-related.
  var seconds = date.getSeconds();
  var secondsLeadingZero = ((seconds < 10) ? '0' + seconds : seconds);
  var beats = Math.floor(((hours * 3600) + (minutes * 60) + seconds - parseInt(offsetSeconds) + 3600) / 86400 * 1000);
  if (beats < 10) {
    var beatsLeadingZero = ((beats < 10) ? '00' + beats : beats);
  }
  else if (beats < 100) {
    var beatsLeadingZero = ((beats < 100) ? '0' + beats : beats);
  }
  else {
    var beatsLeadingZero = beats;
  }
  var timestamp = date.getTime();

  // Turn the date format string into an array so each character has its own key.
  var dateFormat = dateFormat.split('');

  // Perform the date format conversion for a character.
  for (i=0; i < dateFormat.length; i++) {
    switch (dateFormat[i]) {
      // If the escape character '\' is used, simply return the following character.
      case '\\':
        formattedDate += dateFormat[++i];
        break;
      // 'am' or 'pm' depending on the time of day.
      case 'a':
        formattedDate += ((hours >= 12) ? Drupal.t('pm') : Drupal.t('am'));
        break;
      // 'AM' or 'PM' depending on the time of day.
      case 'A':
        formattedDate += ((hours >= 12) ? Drupal.t('PM') : Drupal.t('AM'));
        break;
      // Swatch-Internet-Time.
      case 'B':
        formattedDate += beatsLeadingZero;
        break;
      // ISO-8601 date.
      case 'c':
        formattedDate += year + '-' + monthLeadingZero + '-' + dayLeadingZero + 'T' + hoursLeadingZero + ':' + minutesLeadingZero + ':' + secondsLeadingZero + offsetPrefix + offsetHoursLeadingZero + ':' + offsetMinutesLeadingZero;
        break;
      // Day of month with leading zero, '01' - '31'.
      case 'd':
        formattedDate += dayLeadingZero;
        break;
      // Short name of weekday, e.g. 'Sun'.
      case 'D':
        formattedDate += weekdayShortName;
        break;
      // Time zone name, e.g. 'Europe/London';
      case 'e':
        formattedDate += timezoneName;
        break;
      // Name of month, e.g. 'January'.
      case 'F':
        formattedDate += monthLongName;
        break;
      // Hours in 12-hour format, '1' - '12'.
      case 'g':
        formattedDate += hoursTwelve;
        break;
      // Hours in 24-hour format, '0' - '23'.
      case 'G':
        formattedDate += hours;
        break;
      // Hours in 12-hour format with leading zero, '01' - '12'.
      case 'h':
        formattedDate += hoursTwelveLeadingZero;
        break;
      // Hours in 24-hour format with leading zero, '00' - '23'.
      case 'H':
        formattedDate += hoursLeadingZero;
        break;
      // Minutes with leading zero, '00' - '59'.
      case 'i':
        formattedDate += minutesLeadingZero;
        break;
      // Daylight Savings Time, '1' or '0'
      case 'I':
        formattedDate += daylightSavingsTime;
        break;
      // Day of month, '1' - '31'
      case 'j':
        formattedDate += day;
        break;
      // Name of weekday, e.g. 'Sunday'.
      case 'l':
        formattedDate += weekdayLongName;
        break;
      // Leap year, '1' or '0'.
      case 'L':
        formattedDate += leapYear;
        break;
      // Number of month with leading zero, '01' - '12'
      case 'm':
        formattedDate += monthLeadingZero;
        break;
      // Short name of month, e.g. 'Jan'.
      case 'M':
        formattedDate += monthShortName;
        break;
      // Number of month, '1' - '12'.
      case 'n':
        formattedDate += month;
        break;
      // ISO-8601 weekday number, '1' - '7'.
      case 'N':
        formattedDate += weekday + 1;
        break;
      // ISO-8601 year number.
      // This has a different value from 'Y' in certain constellations at the
      // beginning or end of a year, but this is not implemented here.
      case 'o':
        formattedDate += year;
        break;
      // Time zone offset, e.g. '+1030'
      case 'O':
        formattedDate += offsetPrefix + offsetHoursLeadingZero + offsetMinutesLeadingZero;
        break;
      // Time zone offset, e.g. '+10:30'.
      case 'P':
        formattedDate += offsetPrefix + offsetHoursLeadingZero + ':' + offsetMinutesLeadingZero;
        break;
      // RFC-2822 date.
      case 'r':
        formattedDate += weekdayShortName + ', ' + dayLeadingZero + ' ' + monthShortName + ' ' + year + ' ' + hoursLeadingZero + ':' + minutesLeadingZero + ':' + secondsLeadingZero + ' ' + offsetPrefix + offsetHoursLeadingZero + offsetMinutesLeadingZero;
        break;
      // Seconds with leading zero.
      case 's':
        formattedDate += secondsLeadingZero;
        break;
      // Appendage for the day of month, e.g. 'st' if the day is '1'.
      case 'S':
        formattedDate += dayOfMonthAppend;
        break;
      // Total days of month, '28' - '31'.
      case 't':
        formattedDate += months[month];
        break;
      // Name of offset, e.g. 'GMT'.
      case 'T':
        formattedDate += offsetName;
        break;
      // Milliseconds since the begin of the UNIX-era.
      case 'u':
        formattedDate += timestamp;
        break;
      // Seconds since the begin of the UNIX-era.
      case 'U':
        formattedDate += timestamp / 1000;
        break;
      // Number of weekday, '0' for Sunday, ..., '6' for Saturday.
      case 'w':
        formattedDate += weekday;
        break;
      // ISO-8601 week number, '01' - '52'.
      case 'W':
        formattedDate += weekNumberLeadingZero;
        break;
      // Year, e.g. '2001'.
      case 'Y':
        formattedDate += year;
        break;
      // Short year, e.g. '01'.
      case 'y':
        formattedDate += yearShort;
        break;
      // Days of the year, '0' - '365'.
      // Since PHP starts counting the days of the year at '0', we need subtract one.
      case 'z':
        formattedDate += daysOfYear - 1;
        break;
      // Time zone offset in seconds.
      case 'Z':
        formattedDate += offsetSeconds;
        break;
      // If the character is not a date formatter, just add it to the output.
      default:
        formattedDate += dateFormat[i];
        break;
    }
  }
  return formattedDate;
}

