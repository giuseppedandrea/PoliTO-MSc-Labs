'use strict';

const sqlite = require('sqlite3');

// Open the database
const db = new sqlite.Database("tasks.db", (err) => {
  if (err) {
    console.error(err.message);
    throw err;
  }
  console.log("Connected to the database");
});

db.serialize(() => {
  // Enable Foreign Key constraint (https://stackoverflow.com/questions/53084827)
  db.run("PRAGMA foreign_keys = ON", (err) => {
    if (err) {
      console.error(err.message);
      throw err;
    }
    console.log("Foreign Key constraint is on");
  });
});

module.exports = db;
