import './App.css';

import 'bootstrap/dist/css/bootstrap.min.css';
import { Container, Row, Col, Button, Alert } from 'react-bootstrap';
import { BrowserRouter as Router, Switch, Route, Link, Redirect } from 'react-router-dom';
import { useEffect, useState } from 'react';
import NavBar from './components/NavBar';
import Sidebar from './components/Sidebar';
import TasksList from './components/TasksList';
import TasksForm from './components/TasksForm';
import LoginForm from './components/LoginForm';
import TaskAPI from './api/TaskAPI';
import UserAPI from './api/UserAPI';

const filters = [
  "all",
  "important",
  "today",
  "next-7-days",
  "private"
];

function App() {
  const [openCollapse, setOpenCollapse] = useState(false);
  const [tasks, setTasks] = useState([]);
  const [selectedFilter, setSelectedFilter] = useState(filters[0]); // FIXME If you write directly an URL with a filter, the application redirects to the main route
  const [loading, setLoading] = useState(true);
  const [dirty, setDirty] = useState(true);
  const [message, setMessage] = useState("");
  const [loggedIn, setLoggedIn] = useState(false);
  const [user, setUser] = useState({});

  useEffect(() => {
    const checkAuth = async () => {
      try {
        const user = await UserAPI.getUserInfo();
        setUser(user);
        setLoggedIn(true);
      } catch (err) {
        // handleErrors(err);
      }
    };
    checkAuth();
  }, []);

  useEffect(() => {
    async function getTasks() {
      try {
        setLoading(true);
        const tasks = await TaskAPI.getTasks(selectedFilter);
        setTasks(tasks);
        setLoading(false);
        setDirty(false);
      } catch (err) {
        handleErrors(err);
      }
    };
    if (loggedIn && dirty) {
      getTasks();
    }
  }, [loggedIn, dirty, selectedFilter]);

  const handleErrors = (err) => {
    if (err.errors) {
      setMessage({ msg: err.errors[0].param + ": " + err.errors[0].msg, type: "danger" });
    } else {
      setMessage({ msg: err.error, type: "danger" });
    }
    setDirty(true);
  }

  const addTask = async (task) => {
    task.status = "added";
    setTasks(oldTasks => [...oldTasks, task]);

    try {
      await TaskAPI.addTask(task);
      setDirty(true);
    } catch (err) {
      handleErrors(err);
    }
  };

  const updateTask = async (task) => {
    setTasks(oldTasks => {
      return oldTasks.map(t => {
        if (t.id === task.id) {
          return { ...task, status: "updated" };
        } else {
          return t;
        }
      });
    });

    try {
      await TaskAPI.updateTask(task);
      setDirty(true);
    } catch (err) {
      handleErrors(err);
    }
  };

  const completeTask = async (task) => {
    setTasks(oldTasks => {
      return oldTasks.map(t => {
        if (t.id === task.id) {
          return { ...task, status: "completed" };
        } else {
          return t;
        }
      });
    });

    try {
      await TaskAPI.completeTask(task);
      setDirty(true);
    } catch (err) {
      handleErrors(err);
    }
  };

  const deleteTask = async (task) => {
    setTasks(oldTasks => {
      return oldTasks.map(t => {
        if (t.id === task.id) {
          return { ...task, status: "deleted" };
        } else {
          return t;
        }
      });
    });

    try {
      await TaskAPI.deleteTask(task);
      setDirty(true);
    } catch (err) {
      handleErrors(err);
    }
  };

  const doLogin = async (credentials) => {
    try {
      const user = await UserAPI.login(credentials);
      setLoggedIn(true);
      setUser(user);
    } catch (err) {
      handleErrors(err);
    }
  };

  const doLogout = async () => {
    try {
      await UserAPI.logout();
      setLoggedIn(false);
      setTasks([]);
      setSelectedFilter(filters[0]);
      setLoading(true);
      setDirty(true);
      setMessage("");
      setUser({});
    } catch (err) {
      handleErrors(err);
    }
  };

  const changeFilter = (filter) => {
    setSelectedFilter(filter);
    setDirty(true);
  }

  const toggleCollapse = () => {
    setOpenCollapse((openCollapse) => !openCollapse);
  };

  return (
    <>
      <Router>
        {/* Navbar */}
        <NavBar toggleCollapse={toggleCollapse} name={user.name} doLogout={doLogout} />
        {/* Switch */}
        <Switch>
          {/* Login Route */}
          <Route exact path="/login" render={() =>
            <>
              {/* Login Form */}
              {loggedIn ? <Redirect to="/" /> : <LoginForm message={message} setMessage={setMessage} doLogin={doLogin} />}
            </>
          } />
          {/* Add Route */}
          <Route exact path="/add" render={() =>
            <>
              {/* Task Form */}
              {loggedIn ? <TasksForm manageTask={addTask} /> : <Redirect to="/login" />}
            </>
          } />
          {/* Update Route */}
          <Route exact path="/update/:id" render={() =>
            <>
              {/* Task Form */}
              {loggedIn ? <TasksForm manageTask={updateTask} /> : <Redirect to="/login" />}
            </>
          } />
          {/* Filter Route */}
          <Route exact path="/tasks/:filter" render={({ match }) => {
            const filter = match.params.filter;
            if (filters.includes(filter) === false) {
              return (
                <Redirect to="/" />
              );
            } else {
              return (
                <>
                  {/* Content */}
                  {loggedIn
                    ? <>
                      <Container fluid>
                        <Row className="min-vh-100">
                          {/* Sidebar */}
                          <Sidebar openCollapse={openCollapse} filters={filters} selectedFilter={filter} changeFilter={changeFilter} />
                          {/* Main Content */}
                          <Col md={8} className="pt-nav">
                            {/* Alert */}
                            {message && <Alert className="my-3" variant={message.type} onClose={() => setMessage("")} dismissible>{message.msg}</Alert>}
                            {/* Tasks List */}
                            <TasksList selectedFilter={filter} tasks={tasks} completeTask={completeTask} deleteTask={deleteTask} loading={loading} />
                            {/* Add Task */}
                            <Link to="/add">
                              <Button variant="success" size="lg" className="rounded-circle fixed-bottom-right m-3">+</Button>
                            </Link>
                          </Col>
                        </Row>
                      </Container>
                    </>
                    : <Redirect to="/login" />
                  }
                </>
              );
            }
          }} />
          {/* Default Route */}
          <Route path="/" render={() => {
            return (
              <>
                {/* Content */}
                {loggedIn
                  ? <>
                    <Container fluid>
                      <Row className="min-vh-100">
                        {/* Sidebar */}
                        <Sidebar openCollapse={openCollapse} filters={filters} selectedFilter={filters[0]} changeFilter={changeFilter} />
                        {/* Main Content */}
                        <Col md={8} className="pt-nav">
                          {/* Alert */}
                          {message && <Alert className="my-3" variant={message.type} onClose={() => setMessage("")} dismissible>{message.msg}</Alert>}
                          {/* Tasks List */}
                          <TasksList selectedFilter={filters[0]} tasks={tasks} completeTask={completeTask} deleteTask={deleteTask} loading={loading} />
                          {/* Add Task */}
                          <Link to="/add">
                            <Button variant="success" size="lg" className="rounded-circle fixed-bottom-right m-3">+</Button>
                          </Link>
                        </Col>
                      </Row>
                    </Container>
                  </>
                  : <Redirect to="/login" />
                }
              </>
            );
          }} />
        </Switch>
      </Router>
    </>
  );
}

export default App;
