'use strict';

const AWS            = require('aws-sdk');
const documentClient = new AWS.DynamoDB.DocumentClient();

module.exports.process = async event => {
  console.debug(JSON.stringify(event));

  var params = {
    TableName: process.env.DYNAMODB_TABLE,
    Item: {
      time      : event.time,
      temp      : event.temp,
      humidity  : event.humidity,
      heat_index: event.heat_index
    }
  };

  // write data to the table
  documentClient.put(params, function (err, data) {
    // handle potential errors
    if (err) {
      console.error('Error: ' + err);
      return;
    }
    console.debug('item put with success');
  });

};
