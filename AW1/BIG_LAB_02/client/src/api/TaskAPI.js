import Task from '../models/Task';

const baseURL = "/api/v1";

async function getTasks(filter) {
  let url = baseURL + "/tasks";
  if (filter) {
    const queryParams = "?filter=" + filter;
    url += queryParams;
  }

  return new Promise((resolve, reject) => {
    fetch(url)
      .then((response) => {
        response.json()
          .then((data) => {
            if (response.ok) {
              resolve(data.map((t) => new Task(t.id, t.description, t.isImportant, t.isPrivate, t.isCompleted, t.deadline, t.user)));
            } else {
              reject(data);
            }
          })
          .catch((err) => {
            reject({ errors: [{ param: "Application", msg: "Cannot parse server response" }] });
          });
      })
      .catch((err) => {
        reject({ errors: [{ param: "Server", msg: "Cannot communicate" }] });
      });
  });
};

async function addTask(task) {
  const url = baseURL + "/tasks";
  const init = {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify(task)
  };

  return new Promise((resolve, reject) => {
    fetch(url, init)
      .then((response) => {
        if (response.ok) {
          resolve(null);
        } else {
          response.json()
            .then((obj) => {
              reject(obj);
            })
            .catch((err) => {
              reject({ errors: [{ param: "Application", msg: "Cannot parse server response" }] });
            });
        }
      })
      .catch((err) => {
        reject({ errors: [{ param: "Server", msg: "Cannot communicate" }] });
      });
  });
}

async function updateTask(task) {
  const url = baseURL + "/tasks/" + task.id;
  const init = {
    method: "PUT",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify(task)
  };

  return new Promise((resolve, reject) => {
    fetch(url, init)
      .then((response) => {
        if (response.ok) {
          resolve(null);
        } else {
          response.json()
            .then((obj) => {
              reject(obj);
            })
            .catch((err) => {
              reject({ errors: [{ param: "Application", msg: "Cannot parse server response" }] });
            });
        }
      })
      .catch((err) => {
        reject({ errors: [{ param: "Server", msg: "Cannot communicate" }] });
      });
  });
}

async function completeTask(task) {
  const url = baseURL + "/tasks/" + task.id;
  const init = {
    method: "PUT",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify(task)
  };

  return new Promise((resolve, reject) => {
    fetch(url, init)
      .then((response) => {
        if (response.ok) {
          resolve(null);
        } else {
          response.json()
            .then((obj) => {
              reject(obj);
            })
            .catch((err) => {
              reject({ errors: [{ param: "Application", msg: "Cannot parse server response" }] });
            });
        }
      })
      .catch((err) => {
        reject({ errors: [{ param: "Server", msg: "Cannot communicate" }] });
      });
  });
}

async function deleteTask(task) {
  const url = baseURL + "/tasks/" + task.id;
  const init = {
    method: "DELETE"
  };

  return new Promise((resolve, reject) => {
    fetch(url, init)
      .then((response) => {
        if (response.ok) {
          resolve(null);
        } else {
          response.json()
            .then((obj) => {
              reject(obj);
            })
            .catch((err) => {
              reject({ errors: [{ param: "Application", msg: "Cannot parse server response" }] });
            });
        }
      })
      .catch((err) => {
        reject({ errors: [{ param: "Server", msg: "Cannot communicate" }] });
      });
  });
}

const TaskAPI = { getTasks, addTask, updateTask, completeTask, deleteTask };
export default TaskAPI;
