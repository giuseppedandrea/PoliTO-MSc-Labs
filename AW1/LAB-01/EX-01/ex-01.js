"use strict";

const dayjs = require('dayjs');

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
    this.tasks = [];

    this.add = (task) => {
        this.tasks.push(task);
    };

    this.sortAndPrint = () => {
        const tasksSorted = [...this.tasks].sort((a, b) => {
            if (a.deadline === undefined && b.deadline === undefined) {
                return 0;
            } else if (b.deadline === undefined) {
                return -1;
            } else if (a.deadline === undefined) {
                return 1;
            } else {
                return a.deadline.diff(b.deadline, 'minute');
            }
        });
        console.log('****** Tasks sorted by deadline (most recent first): ******');
        tasksSorted.forEach((task) => {console.log(task.toString())});
    };

    this.filterAndPrint = () => {
        const tasksFiltered = this.tasks.filter((task) => (task.urgentTask));
        console.log('****** Tasks filtered, only (urgent == true): ******');
        tasksFiltered.forEach((task) => {console.log(task.toString())});
    };
};


const tasks = [];

tasks.push(new Task(1, 'laundry'));
tasks.push(new Task(2, 'monday lab', false, false, dayjs('2021-03-16 10:00 AM')));
tasks.push(new Task(3, 'phone call', true, false, dayjs('2021-03-08 4:20 PM')));

const taskList = new TaskList();

tasks.forEach((task) => {taskList.add(task)});

taskList.sortAndPrint();

taskList.filterAndPrint();

debugger;
