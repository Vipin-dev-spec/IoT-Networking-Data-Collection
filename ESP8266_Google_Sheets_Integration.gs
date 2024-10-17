function doGet(e) {
  Logger.log(JSON.stringify(e)); // Log the request parameters

  var result = 'Ok'; // Default result

  // Check if there are parameters in the request
  if (e.parameter == 'undefined') {
    result = 'No Parameters'; // Update result if no parameters are found
  } else {
    var sheet_id = 'EXCEL_SHEET_ID'; // Spreadsheet ID
    var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet(); // Get the active sheet

    // Get the last row and prepare for new data insertion
    var newRow = sheet.getLastRow() + 1;
    var rowData = [];

    // Get current date and time
    var Curr_Date = new Date();
    rowData[0] = Curr_Date; // Date in column A
    var Curr_Time = Utilities.formatDate(Curr_Date, "GMT+5:30", 'HH:mm:ss');
    rowData[1] = Curr_Time; // Time in column B

    // Loop through each parameter in the request
    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]); // Remove quotes from parameter value
      Logger.log(param + ':' + e.parameter[param]);

      // Check each parameter and update corresponding column in the spreadsheet
      switch (param) {
        case 'distance_cm':
          rowData[4] = value; // Distance in centimeter on Column E
          result = 'Distance in cm Written on column E';
          break;
        case 'distance_inch':
          rowData[5] = value; // Distance in meter on column F
          result += ' ,Distance in m Written on column F';
          break;
        case 'temperature':
          rowData[2] = value; // Temperature in column C
          result = 'Temperature Written on column C';
          break;
        case 'humidity':
          rowData[3] = value; // Humidity in column D
          result += ' ,Humidity Written on column D';
          break;
        default:
          result = "unsupported parameter";
      }
    }

    Logger.log(JSON.stringify(rowData)); // Log the updated row data

    // Write the updated data to the spreadsheet
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }

  // Return the result as a text output
  return ContentService.createTextOutput(result);
}

// Function to strip quotes from a value
function stripQuotes(value) {
  return value.replace(/^["']|['"]$/g, "");
}
