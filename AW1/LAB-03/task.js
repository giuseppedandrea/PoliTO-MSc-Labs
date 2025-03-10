"use strict";

function Task(id, description, important = false, pvt = true, deadline) {
    this.id = id;
    this.description = description;
    this.important = important;
    this.pvt = pvt;
    this.deadline = deadline;

    this.toString = () => {
        return `ID: ${this.id}, Description: ${this.description}, Important: ${this.important}, Private: ${this.pvt}, Deadline: ${this.deadline}`;
    };

    this.createNode = () => {
        const li = document.createElement("li");
        li.className = "list-group-item";
        li.setAttribute("id", `task-${this.id}`);
        
        const divRow = document.createElement("div");
        divRow.className = "d-flex w-100 justify-content-between";
        li.appendChild(divRow);
        
        const divCheckboxDescription = document.createElement("div");
        divRow.appendChild(divCheckboxDescription);
        
        const inputCheckbox = document.createElement("input");
        inputCheckbox.className = "form-check-inline";
        inputCheckbox.setAttribute("type", "checkbox");
        inputCheckbox.setAttribute("value", "");
        inputCheckbox.setAttribute("id", `ck-${this.id}`);
        divCheckboxDescription.appendChild(inputCheckbox);
        
        const labelDescription = document.createElement("label");
        if (this.important === false) {
            labelDescription.className = "form-check-label";
        } else {
            labelDescription.className = "form-check-label important";
        }
        labelDescription.setAttribute("for", `ck-${this.id}`);
        labelDescription.innerText = this.description;
        divCheckboxDescription.appendChild(labelDescription);
        
        if (this.pvt === false) {
            divCheckboxDescription.insertAdjacentHTML("afterend",
                `<svg xmlns="http://www.w3.org/2000/svg" width="1.2em" height="1.2em" fill="currentColor" class="bi bi-person-square" viewBox="0 0 16 16">
                    <path d="M11 6a3 3 0 1 1-6 0 3 3 0 0 1 6 0z" />
                    <path d="M2 0a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V2a2 2 0 0 0-2-2H2zm12 1a1 1 0 0 1 1 1v12a1 1 0 0 1-1 1v-1c0-1-1-4-6-4s-6 3-6 4v1a1 1 0 0 1-1-1V2a1 1 0 0 1 1-1h12z" />
                </svg>`)
        }
        
        if (this.deadline !== undefined) {
            const spanDeadline = document.createElement("span");
            spanDeadline.className = "small";
            spanDeadline.innerText = `${this.deadline.format("dddd DD MMMM YYYY")} at ${this.deadline.format("HH:mm")}`;
            divRow.appendChild(spanDeadline);
        }
        
        document.getElementById("tasks").appendChild(li);
    };
};

function TaskList() {
    this.tasks = [];

    this.add = (task) => {
        this.tasks.push(task);

        console.log('*** Task inserted ***');
        console.log(task.toString());
    };

    this.sortByDeadline = () => {
        const tasks = [...this.tasks].sort((a, b) => {
            if (a.deadline === undefined && b.deadline === undefined) {
                return 0;
            } else if (b.deadline === undefined) {
                return -1;
            } else if (a.deadline === undefined) {
                return 1;
            } else {
                return a.deadline.diff(b.deadline, "minute");
            }
        });

        console.log('*** Tasks sorted by deadline (most recent first): ***');
        tasks.forEach((task) => { console.log(task.toString()) });

        return tasks;
    };

    this.filterAll = () => {
        const tasks = [...this.tasks];

        console.log('*** Tasks filtered, all: ***');
        tasks.forEach((task) => { console.log(task.toString()) });

        return tasks;
    };

    this.filterByImportant = () => {
        const tasks = [...this.tasks].filter((task) => (task.important));

        console.log('*** Tasks filtered, only (important == true): ***');
        tasks.forEach((task) => { console.log(task.toString()) });

        return tasks;
    };

    this.filterByPrivate = () => {
        const tasks = [...this.tasks].filter((task) => (task.pvt));

        console.log('*** Tasks filtered, only (private == true): ***');
        tasks.forEach((task) => { console.log(task.toString()) });

        return tasks;
    };

    this.filterByDeadlineToday = () => {
        const tasks = [...this.tasks].filter((task) => {
            const now = dayjs();
            if (task.deadline !== undefined) {
                return task.deadline.diff(now, "day") === 0;
            } else {
                return false;
            }
        });

        console.log('*** Tasks filtered, only (deadline == Today): ***');
        tasks.forEach((task) => { console.log(task.toString()) });

        return tasks;
    };

    this.filterByDeadlineNextWeek = () => {
        const tasks = [...this.tasks].filter((task) => {
            const now = dayjs();
            if (task.deadline !== undefined) {
                return task.deadline.diff(now, "day") <= 7;
            } else {
                return false;
            }
        });
        console.log('*** Tasks filtered, only (deadline == Next Week): ***');
        tasks.forEach((task) => { console.log(task.toString()) });

        return tasks;
    };
};

const taskList = new TaskList();

const filterTitle = document.getElementById("filter-title");
const filterAll = document.getElementById("filter-all");
const filterImportant = document.getElementById("filter-important");
const filterToday = document.getElementById("filter-today");
const filterNextWeek = document.getElementById("filter-next-week");
const filterPrivate = document.getElementById("filter-private");

window.addEventListener("load", (event) => {
    let id = 0;
    taskList.add(new Task(++id, 'Laundry'));
    taskList.add(new Task(++id, 'Monday lab', false, false, dayjs('2021-04-16 10:00 AM')));
    taskList.add(new Task(++id, 'Phone call', true, true, dayjs('2021-04-02 4:20 PM')));
    taskList.add(new Task(++id, 'Complete Lab 3', true, false, dayjs('2021-03-29 20:00')));

    taskList.filterAll().forEach((task) => {
        task.createNode();
    });
});

filterAll.addEventListener("click", (event) => {
    const currentFilter = document.querySelector("#sidebar button.active")
    if (currentFilter !== filterAll) {
        currentFilter.classList.remove("active");
        filterAll.classList.add("active");

        filterTitle.innerText = "All";

        document.getElementById("tasks").innerHTML = "";
        taskList.filterAll().forEach((task) => {
            task.createNode();
        });
    }
});

filterImportant.addEventListener("click", (event) => {
    const currentFilter = document.querySelector("#sidebar button.active")
    if (currentFilter !== filterImportant) {
        currentFilter.classList.remove("active");
        filterImportant.classList.add("active");

        filterTitle.innerText = "Important";

        document.getElementById("tasks").innerHTML = "";
        taskList.filterByImportant().forEach((task) => {
            task.createNode();
        });
    }
});

filterToday.addEventListener("click", (event) => {
    const currentFilter = document.querySelector("#sidebar button.active")
    if (currentFilter !== filterToday) {
        currentFilter.classList.remove("active");
        filterToday.classList.add("active");

        filterTitle.innerText = "Today";

        document.getElementById("tasks").innerHTML = "";
        taskList.filterByDeadlineToday().forEach((task) => {
            task.createNode();
        });
    }
});

filterNextWeek.addEventListener("click", (event) => {
    const currentFilter = document.querySelector("#sidebar button.active")
    if (currentFilter !== filterNextWeek) {
        currentFilter.classList.remove("active");
        filterNextWeek.classList.add("active");

        filterTitle.innerText = "Next 7 Days";

        document.getElementById("tasks").innerHTML = "";
        taskList.filterByDeadlineNextWeek().forEach((task) => {
            task.createNode();
        });
    }
});

filterPrivate.addEventListener("click", (event) => {
    const currentFilter = document.querySelector("#sidebar button.active")
    if (currentFilter !== filterPrivate) {
        currentFilter.classList.remove("active");
        filterPrivate.classList.add("active");

        filterTitle.innerText = "Private";

        document.getElementById("tasks").innerHTML = "";
        taskList.filterByPrivate().forEach((task) => {
            task.createNode();
        });
    }
});
