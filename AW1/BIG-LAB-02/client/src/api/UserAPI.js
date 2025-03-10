const baseURL = "/api/v1";

async function login(credentials) {
  const url = baseURL + "/sessions";
  const init = {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify(credentials)
  };

  return new Promise((resolve, reject) => {
    fetch(url, init)
      .then((response) => {
        response.json()
          .then((data) => {
            if (response.ok) {
              resolve(data);
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
}

async function logout() {
  const url = baseURL + "/sessions/current";
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

async function getUserInfo() {
  const url = baseURL + "/sessions/current";

  return new Promise((resolve, reject) => {
    fetch(url)
      .then((response) => {
        response.json()
          .then((data) => {
            if (response.ok) {
              resolve(data);
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

}

const UserAPI = { login, logout, getUserInfo };
export default UserAPI;
