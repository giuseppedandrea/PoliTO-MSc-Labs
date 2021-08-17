'use strict';
/* Data Access Object (DAO) module for accessing tasks */

const db = require('./db');
const dayjs = require('dayjs');

// Retrieve the list of all the available tasks
// Retrieve a list of all the tasks that fulfill a given "filter"
exports.listTasks = (filter, userId) => {
  return new Promise((resolve, reject) => {
    const sql = "SELECT * FROM tasks WHERE user = ?";

    db.all(sql, [userId], (err, rows) => {
      if (err) {
        reject(err);
        return;
      }

      let tasks = rows.map((row) => (
        {
          id: row.id,
          description: row.description,
          isImportant: row.important,
          isPrivate: row.private,
          deadline: row.deadline,
          isCompleted: row.completed,
          user: row.user
        }
      ))

      if (filter) {
        switch (filter) {
          case "all":
            break;
          case "important":
            tasks = tasks.filter((task) => {
              return task.isImportant;
            });
            break;
          case "today":
            const now = dayjs();
            tasks = tasks.filter((task) => {
              return task.deadline && (dayjs(task.deadline).format("YYYY-MM-DD") === now.format("YYYY-MM-DD"));
            });
            break;
          case "next-7-days":
            const tomorrow = dayjs().add(1, "day");
            const nextWeek = dayjs().add(7, "day");
            tasks = tasks.filter((task) => {
              return task.deadline && (!dayjs(task.deadline).isBefore(tomorrow, "day") && !dayjs(task.deadline).isAfter(nextWeek, "day"));
            });
            break;
          case "private":
            tasks = tasks.filter((task) => {
              return task.isPrivate;
            });
            break;
          default:
            tasks = null;
            break;
        }
      }

      resolve(tasks);
    });
  });
};

// Retrieve a task, given its "id"
exports.getTask = (id, userId) => {
  return new Promise((resolve, reject) => {
    const sql = "SELECT * FROM tasks WHERE id = ? AND user = ?";
    db.get(sql, [id, userId], (err, row) => {
      if (err) {
        reject(err);
        return;
      }

      if (row === undefined) {
        resolve(null);
        return;
      }

      const task = {
        id: row.id,
        description: row.description,
        isImportant: row.important,
        isPrivate: row.private,
        deadline: row.deadline,
        isCompleted: row.completed,
        user: row.user
      };
      resolve(task);
    });
  });
};

// Create a new task, by providing all relevant information (except the "id")
exports.createTask = (task) => {
  return new Promise((resolve, reject) => {
    const sql = "INSERT INTO tasks(description, important, private, deadline, completed, user) VALUES(?, ?, ?, ?, ?, ?)";
    db.run(sql, [task.description, task.isImportant, task.isPrivate, task.deadline, task.isCompleted, task.user], function (err) {
      if (err) {
        reject(err);
        return;
      }

      resolve(this.lastID);
    });
  });
};

// Update an existing task, by providing all relevant information (all the properties except the "id" will overwrite the current properties of the existing task)
exports.updateTask = (task, userId) => {
  return new Promise((resolve, reject) => {
    const sql = "UPDATE tasks SET description = ?, important = ?, private = ?, deadline = ?, completed = ? WHERE id = ? AND user = ?";
    db.run(sql, [task.description, task.isImportant, task.isPrivate, task.deadline, task.isCompleted, task.id, userId], function (err) {
      if (err) {
        reject(err);
        return;
      }

      resolve(null);
    });
  });
};

// // Mark an existing task as completed/uncompleted
exports.markTask = (task, userId) => {
  return new Promise((resolve, reject) => {
    const sql = "UPDATE tasks SET completed = ? WHERE id = ? AND user = ?";
    db.run(sql, [task.isCompleted, task.id, userId], function (err) {
      if (err) {
        reject(err);
        return;
      }

      resolve(null);
    });
  });
};

// Delete an existing task, given its "id"
exports.deleteTask = (id, userId) => {
  return new Promise((resolve, reject) => {
    const sql = "DELETE FROM tasks WHERE id = ? AND user = ?";
    db.run(sql, [id, userId], function (err) {
      if (err) {
        reject(err);
        return;
      }

      resolve(null);
    });
  });
}
