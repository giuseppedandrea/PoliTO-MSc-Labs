'use strict';
/* Data Access Object (DAO) module for accessing users */

const db = require('./db');
const bcrypt = require('bcrypt');

// Retrieve the user, given its "id"
exports.getUserById = (id) => {
  return new Promise((resolve, reject) => {
    const sql = "SELECT * FROM users WHERE id = ?";
    db.get(sql, [id], (err, row) => {
      if (err) {
        reject(err);
        return;
      }

      if (row === undefined) {
        resolve(null);
        return;
      }

      const user = { id: row.id, username: row.email, name: row.name }
      resolve(user);
    });
  });
};

// Retrieve the user, given its "email" and "password"
exports.getUser = (email, password) => {
  return new Promise((resolve, reject) => {
    const sql = "SELECT * FROM users WHERE email = ?";
    db.get(sql, [email], (err, row) => {
      if (err) {
        reject(err);
        return;
      }

      if (row === undefined) {
        resolve(null);
        return;
      }

      const user = { id: row.id, username: row.email, name: row.name };

      bcrypt.compare(password, row.hash).then(result => {
        if (result) {
          resolve(user);
        } else {
          resolve(null);
        }
      });
    });
  });
};
