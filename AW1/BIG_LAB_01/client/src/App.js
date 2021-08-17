import './App.css';

import 'bootstrap/dist/css/bootstrap.min.css';
import { Container, Row, Col, Button } from 'react-bootstrap';
import { BrowserRouter as Router, Switch, Route, Link, Redirect } from 'react-router-dom'
import { useState } from 'react';
import NavBar from './components/NavBar';
import Sidebar from './components/Sidebar';
import TasksList from './components/TasksList';
import TasksForm from './components/TasksForm';
import dayjs from 'dayjs';

const dummyTasks = [
  { id: 1, description: "Buy some groceries", isUrgent: false, isPrivate: true, deadline: "" },
  { id: 2, description: "Call John", isUrgent: false, isPrivate: true, deadline: dayjs("2021-05-08T18:00:00") },
  { id: 3, description: "Complete BigLab1", isUrgent: true, isPrivate: false, deadline: dayjs("2021-05-09T23:59:00") },
  { id: 4, description: "Laundry", isUrgent: false, isPrivate: true, deadline: dayjs("2021-05-21T17:00:00") }
];

const filters = {
  "all": (task) => {
    return task;
  },
  "important": (task) => {
    return task.isUrgent;
  },
  "today": (task) => {
    const now = dayjs();
    return task.deadline && (task.deadline.format("YYYY-MM-DD") === now.format("YYYY-MM-DD"));
  },
  "next-7-days": (task) => {
    const tomorrow = dayjs().add(1, "day");
    const nextWeek = dayjs().add(7, "day");
    return task.deadline && (!task.deadline.isBefore(tomorrow, "day") && !task.deadline.isAfter(nextWeek, "day"));
  },
  "private": (task) => {
    return task.isPrivate;
  }
};

function App() {
  const [openCollapse, setOpenCollapse] = useState(false);
  const [tasks, setTasks] = useState([...dummyTasks]);

  const toggleCollapse = () => {
    setOpenCollapse((openCollapse) => !openCollapse);
  }

  const addTask = (task) => {
    setTasks((oldTasks) => [...oldTasks, task]);
  }

  const deleteTask = (task) => {
    setTasks((oldTasks) => oldTasks.filter((t => (t.id !== task.id))));
  }

  const updateTask = (task) => {
    setTasks((oldTasks) => oldTasks.map((t) => {
      if (t.id === task.id) {
        return task;
      } else {
        return t;
      }
    }))
  }

  const getLastId = () => {
    return [...tasks].sort((a, b) => (a.id - b.id))[tasks.length - 1].id;
  }

  return (
    <>
      <Router>
        {/* Navbar */}
        <NavBar toggleCollapse={toggleCollapse} />
        {/* Switch */}
        <Switch>
          {/* Add Route */}
          <Route exact path="/add" render={() =>
            <>
              {/* Task Form */}
              <TasksForm manageTask={addTask} getLastId={getLastId} />
            </>
          } />
          {/* Update Route */}
          <Route exact path="/update/:id" render={() =>
            <>
              {/* Task Form */}
              <TasksForm manageTask={updateTask} />
            </>
          } />
          {/* Filter Route */}
          <Route exact path="/tasks/:filter" render={({ match }) => {
            const filter = match.params.filter;
            if (Object.keys(filters).includes(filter) === false) {
              return (
                <Redirect to="/" />
              );
            } else {
              return (
                <>
                  {/* Content */}
                  <Container fluid>
                    <Row className="min-vh-100">
                      {/* Sidebar */}
                      <Sidebar openCollapse={openCollapse} filters={filters} selectedFilter={filter} />
                      {/* Main Content */}
                      <Col md={8} className="pt-nav">
                        {/* Tasks List */}
                        <TasksList filters={filters} selectedFilter={filter} tasks={tasks} deleteTask={deleteTask} />
                        {/* Add Task */}
                        <Link to="/add">
                          <Button variant="success" size="lg" className="rounded-circle fixed-bottom-right m-3">+</Button>
                        </Link>
                      </Col>
                    </Row>
                  </Container>
                </>
              );
            }
          }} />
          {/* Default Route */}
          <Route path="/" render={() => {
            return (
              <>
                {/* Content */}
                <Container fluid>
                  <Row className="min-vh-100">
                    {/* Sidebar */}
                    <Sidebar openCollapse={openCollapse} filters={filters} selectedFilter={Object.keys(filters)[0]} />
                    {/* Main Content */}
                    <Col md={8} className="pt-nav">
                      {/* Tasks List */}
                      <TasksList filters={filters} selectedFilter={Object.keys(filters)[0]} tasks={tasks} deleteTask={deleteTask} />
                      {/* Add Task */}
                      <Link to="/add">
                        <Button variant="success" size="lg" className="rounded-circle fixed-bottom-right m-3">+</Button>
                      </Link>
                    </Col>
                  </Row>
                </Container>
              </>
            );
          }} />
        </Switch>
      </Router>
    </>
  );
}

export default App;
