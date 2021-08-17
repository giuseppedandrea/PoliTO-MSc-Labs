const express = require('express');
const morgan = require('morgan');
const passport = require('passport');
const LocalStrategy = require('passport-local').Strategy;
const session = require('express-session');
const { check, oneOf, validationResult } = require('express-validator');
const tasksDAO = require('./tasksDAO');
const usersDAO = require('./usersDAO');

// Init express
const port = 3001;
const app = new express();

// Setup Passport
passport.use(new LocalStrategy(
  function(username, password, done) {
    usersDAO.getUser(username, password).then((user) => {
      if (!user) {
        return done(null, false, "Incorrect username and/or password");
      }
      return done(null, user);
    }).catch((err) => {
      return done(err);
    });
  }
));

passport.serializeUser((user, done) => {
  done(null, user.id);
});

passport.deserializeUser((id, done) => {
  usersDAO.getUserById(id)
    .then(user => {
      done(null, user);
    }).catch((err) => {
      done(err);
    });
});

// Middlewares
app.use(morgan("dev"));
app.use(express.json());
app.use(session({
  secret: "Nel mezzo del cammin di nostra vita mi ritrovai per una selva oscura ché la diritta via era smarrita.",
  resave: false,
  saveUninitialized: false
}));
app.use(passport.initialize());
app.use(passport.session());

// Custom middlewares
const isLoggedIn = (req, res, next) => {
  if (req.isAuthenticated()) {
    return next();
  }
  
  return res.status(401).json({errors: [{ param: "Server", msg: "Unauthenticated user" }]});
}


/** Tasks APIs **/

const baseURL = "/api/v1";

// Retrieve the list of all the available tasks
// GET /api/v1/tasks
// Retrieve a list of all the tasks that fulfill a given filter
// GET /api/v1/tasks?filter=<filter>
app.get(`${baseURL}/tasks`, isLoggedIn, async (req, res) => {
  try {
    const tasks = await tasksDAO.listTasks(req.query.filter, req.user.id);
    if (!tasks) {
      res.status(404).json({ errors: [{ param: "Server", msg: "Filter not found" }] });
      return;
    }
    // res.status(200).json(tasks);
    setTimeout(() => {res.status(200).json(tasks)}, 2000); // DEBUG
  } catch (err) {
    res.status(500).json({ errors: [{ param: "Server", msg: "Internal error" }] });
  }
});

// Retrieve a task, given its "id"
// GET /api/v1/tasks/<id>
app.get(`${baseURL}/tasks/:id([0-9]+)`, isLoggedIn, async (req, res) => {
  try {
    const task = await tasksDAO.getTask(req.params.id, req.user.id);
    if (!task) {
      res.status(404).json({ errors: [{ param: "Server", msg: "Task not found" }] });
      return;
    }
    // res.status(200).json(task);
    setTimeout(() => {res.status(200).json(task)}, 2000); // DEBUG
  } catch (err) {
    res.status(500).json({ errors: [{ param: "Server", msg: "Internal error" }] });
  }
});

// Create a new task, by providing all relevant information (except the "id")
// POST /api/v1/tasks
app.post(`${baseURL}/tasks`, isLoggedIn, oneOf([
  [
    check("description").isLength({ min: 1 }),
    check("isImportant").isBoolean(),
    check("isPrivate").isBoolean(),
    check("deadline").isISO8601({ strict: true, strictSeparator: true }),
    check("isCompleted").isBoolean()
  ],
  [
    check("description").isLength({ min: 1 }),
    check("isImportant").isBoolean(),
    check("isPrivate").isBoolean(),
    check("deadline").isEmpty(),
    check("isCompleted").isBoolean()
  ]
]), async (req, res) => {
  const errors = validationResult(req);
  if (!errors.isEmpty()) {
    res.status(422).json({ errors: errors.array() });
    return;
  }

  const task = {
    description: req.body.description,
    isImportant: req.body.isImportant,
    isPrivate: req.body.isPrivate,
    deadline: req.body.deadline,
    isCompleted: req.body.isCompleted,
    user: req.user.id
  };
  try {
    const id = await tasksDAO.createTask(task);
    // res.status(201).json({ id: id });
    setTimeout(() => {res.status(201).json({ id: id })}, 2000); // DEBUG
  } catch (err) {
    res.status(500).json({ errors: [{ param: "Server", msg: "Internal error" }] });
  }
});

// Update an existing task, by providing all relevant information (all the properties except the "id" will overwrite the current properties of the existing task)
// Mark an existing task as completed/uncompleted
// PUT /api/v1/tasks/<id>
app.put(`${baseURL}/tasks/:id([0-9]+)`, isLoggedIn, oneOf([
  [
    check("description").isLength({ min: 1 }),
    check("isImportant").isBoolean(),
    check("isPrivate").isBoolean(),
    check("deadline").isISO8601({ strict: true, strictSeparator: true }),
    check("isCompleted").isBoolean()
  ],
  [
    check("isCompleted").isBoolean()
  ]
]), async (req, res) => {
  try {
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
      res.status(422).json({ errors: errors.array() });
      return;
    }

    const checkTask = await tasksDAO.getTask(req.params.id, req.user.id);
    if (!checkTask) {
      res.status(404).json({ errors: [{ param: "Server", msg: "Task not found" }] });
      return;
    }

    const task = {
      id: req.params.id,
      description: req.body.description,
      isImportant: req.body.isImportant,
      isPrivate: req.body.isPrivate,
      deadline: req.body.deadline,
      isCompleted: req.body.isCompleted
    };

    if (task.description !== undefined &&
      task.isImportant !== undefined &&
      task.isPrivate !== undefined &&
      task.deadline !== undefined &&
      task.isCompleted !== undefined) {
      await tasksDAO.updateTask(task, req.user.id);
    } else {
      await tasksDAO.markTask(task, req.user.id);
      
    }
    // res.status(204).end();
    setTimeout(() => {res.status(204).end()}, 2000); // DEBUG
  } catch (err) {
    res.status(500).json({ errors: [{ param: "Server", msg: "Internal error" }] });
  }
});

// Delete an existing task, given its "id"
// DELETE /api/v1/tasks/<id>
app.delete(`${baseURL}/tasks/:id([0-9]+)`, isLoggedIn, async (req, res) => {
  try {
    const checkTask = await tasksDAO.getTask(req.params.id, req.user.id);
    if (!checkTask) {
      res.status(404).json({ errors: [{ param: "Server", msg: "Task not found" }] });
      return;
    }

    await tasksDAO.deleteTask(req.params.id, req.user.id);
    // res.status(204).end()
    setTimeout(() => {res.status(204).end()}, 2000); // DEBUG
  } catch (err) {
    res.status(500).json({ errors: [{ param: "Server", msg: "Internal error" }] });
  }
});


/** User APIs **/

// Login
// POST /api/v1/sessions
app.post(`${baseURL}/sessions`, function (req, res, next) {
  passport.authenticate("local", (err, user, info) => {
    if (err) {
      return res.status(500).json({ errors: [{ param: "Server", msg: "Internal error" }] });
    }
    if (!user) {
      return res.status(401).json({errors: [{ param: "Server", msg: info }]});
    }
    req.login(user, (err) => {
      if (err) {
        return res.status(500).json({ errors: [{ param: "Server", msg: "Internal error" }] });
      }
      return res.status(200).json(req.user);
    });
  })(req, res, next);
});

// Logout
// DELETE /api/v1/sessions/current
app.delete(`${baseURL}/sessions/current`, (req, res) => {
  req.logout();
  res.status(204).end();
});

// Check whether the user is logged in or not
// GET /api/v1/sessions/current
app.get(`${baseURL}/sessions/current`, isLoggedIn, (req, res) => {
  res.status(200).json(req.user);
});


// Activate the server
app.listen(port, () => console.log(`BigLab2 server listening at http://localhost:${port}/`));
