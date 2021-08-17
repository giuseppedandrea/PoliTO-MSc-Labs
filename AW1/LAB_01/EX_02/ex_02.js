"use strict";

const dayjs = require('dayjs');
const sqlite = require('sqlite3');

function Task(id, description, urgentTask=false, privateTask=true, deadline) {
    this.id = id;
    this.description = description;
    this.urgentTask = urgentTask;
    this.privateTask = privateTask;
    this.deadline = deadline;

    this.toString = () => {
        return `ID: ${this.id}, Description: ${this.description}, Urgent: ${this.urgentTask}, Private: ${this.privateTask}, Deadline: ${this.deadline}`;
    };
};

function TaskList() {
    this.open = () => {
        return new Promise((resolve, reject) => {
            this.db = new sqlite.Database('tasks.sqlite', (err) => {
                if (err) {
                    reject(err);
                } else {
                    resolve('Ok');
                }
            });
        });
    };

    this.close = () => {
        return new Promise((resolve, reject) => {
            this.db.close((err) => {
                if (err) {
                    reject(err);
                } else {
                    resolve('Ok');
                }
            });
        });
    };

    this.loadAll = () => {
        return new Promise((resolve, reject) => {
            const query = 'SELECT * FROM tasks';
            this.db.each(query, (err, row) => {
                if (err) {
                    reject(err);
                } else {
                    const task = new Task(row.id, row.description, row.urgent, row.private, row.deadline !== null ? dayjs(row.deadline) : undefined);
                    console.log(task.toString());
                    resolve(row);
                }
            });
        });
    };

    this.loadDeadline = (date) => {
        return new Promise((resolve, reject) => {
            const query = 'SELECT * FROM tasks WHERE deadline > DATETIME(?)';
            this.db.each(query, [date.format('YYYY-MM-DD HH:mm:ss')], (err, row) => {
                if (err) {
                    reject(err);
                } else {
                    const task = new Task(row.id, row.description, row.urgent, row.private, row.deadline !== null ? dayjs(row.deadline) : undefined);
                    console.log(task.toString());
                    resolve(row);
                }
            });
        });
    };

    this.loadDescription = (description) => {
        return new Promise((resolve, reject) => {
            const query = 'SELECT * FROM tasks WHERE description LIKE ?';
            this.db.each(query, ['%' + description + '%'], (err, row) => {
                if (err) {
                    reject(err);
                } else {
                    const task = new Task(row.id, row.description, row.urgent, row.private, row.deadline !== null ? dayjs(row.deadline) : undefined);
                    console.log(task.toString());
                    resolve(row);
                }
            });
        });
    };
};

const main = async () => {
    try {
        const taskList = new TaskList();
        await taskList.open();
        console.log('****** Tasks loaded ******');
        await taskList.loadAll();
        await taskList.close();
    } catch (err) {
        console.log(err);
    }

    try {
        const taskList = new TaskList();
        await taskList.open();
        console.log('****** Tasks loaded, only (deadline > 2021-03-09): ******');
        await taskList.loadDeadline(dayjs('2021-03-09'));
        await taskList.close();
    } catch (err) {
        console.log(err);
    }

    try {
        const taskList = new TaskList();
        await taskList.open();
        console.log('****** Tasks loaded, only (description contains \'call\'): ******');
        await taskList.loadDescription('call');
        await taskList.close();
    } catch (err) {
        console.log(err);
    }
};

main();
