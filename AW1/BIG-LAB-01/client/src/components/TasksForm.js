import { Button, Modal, Form, Alert } from 'react-bootstrap';
import { Redirect, useHistory, useLocation } from 'react-router-dom';
import { useState } from 'react';
import dayjs from 'dayjs';

function TasksAdd(props) {
  const history = useHistory();
  const location = useLocation();
  const [description, setDescription] = useState(location.state ? location.state.description : "");
  const [isUrgent, setIsUrgent] = useState(location.state ? location.state.isUrgent : false);
  const [isPrivate, setIsPrivate] = useState(location.state ? location.state.isPrivate : true);
  const [date, setDate] = useState(location.state && location.state.deadline ? location.state.deadline.format("YYYY-MM-DD") : "");
  const [time, setTime] = useState(location.state && location.state.deadline ? location.state.deadline.format("HH:mm") : "");
  const [errorDescription, setErrorDescription] = useState("");
  const [errorDate, setErrorDate] = useState("");
  const [errorTime, setErrorTime] = useState("");
  const [submitted, setSubmitted] = useState(false);

  const handleChangeDescription = (event) => {
    if (errorDescription) setErrorDescription("");
    setDescription(event.target.value);
  }

  const handleChangeIsUrgent = () => {
    setIsUrgent((isUrgent) => !isUrgent);
  }

  const handleChangeIsPrivate = () => {
    setIsPrivate((isPrivate) => !isPrivate);
  }

  const handleChangeDate = (event) => {
    if (errorDate) setErrorDate("");
    if (errorTime) setErrorTime("");
    setDate(event.target.value);
  }

  const handleChangeTime = (event) => {
    if (errorDate) setErrorDate("");
    if (errorTime) setErrorTime("");
    setTime(event.target.value);
  }

  const handleSubmit = (event) => {
    event.preventDefault();

    let correctForm = true;
    if (description === "") {
      setErrorDescription("Please enter a description");
      correctForm = false;
    }

    let deadline = "";
    if (date) {
      const now = dayjs();
      if (time) {
        deadline = dayjs(`${date}T${time}`);
        if (deadline.isSame(now, "day") && deadline.isBefore(now)) {
          setErrorTime("Inserted past time")
          correctForm = false;
        }
        if (!deadline.isSame(now, "day") && deadline.isBefore(now)) {
          setErrorDate("Inserted past date")
          correctForm = false;
        }
      } else {
        deadline = dayjs(`${date}T23:59:59`);
        if (deadline.isBefore(now)) {
          setErrorDate("Inserted past date")
          correctForm = false;
        }
      }
    }

    if (correctForm) {
      const id = location.state ? location.state.id : (props.getLastId() + 1);

      const task = { id: id, description: description, isUrgent: isUrgent, isPrivate: isPrivate, deadline: deadline }

      props.manageTask(task);

      setSubmitted(true);
    }
  }

  return (
    <>
      {submitted ? <Redirect to="/" /> :
        <Modal show={true} onHide={() => { history.goBack() }} animation={false}>
          <Form>
            <Modal.Header closeButton>
              <Modal.Title>{location.state ? "Update" : "Add"} Task</Modal.Title>
            </Modal.Header>
            <Modal.Body>
              {errorDescription && <Alert variant="danger" onClose={() => setErrorDescription("")} dismissible>{errorDescription}</Alert>}
              <Form.Group controlId="controlDescription">
                <Form.Label>Description:</Form.Label>
                <Form.Control as="textarea" rows={2} value={description} onChange={handleChangeDescription} />
              </Form.Group>
              <Form.Group controlId="controlUrgent">
                <Form.Check type="checkbox" label="Urgent" checked={isUrgent} onChange={handleChangeIsUrgent} />
              </Form.Group>
              <Form.Group controlId="controlPrivate">
                <Form.Check type="checkbox" label="Private" checked={isPrivate} onChange={handleChangeIsPrivate} />
              </Form.Group>
              {errorDate && <Alert variant="danger" onClose={() => setErrorDate("")} dismissible>{errorDate}</Alert>}
              <Form.Group controlId="controlDate">
                <Form.Label>Date (Optional):</Form.Label>
                <Form.Control type="date" value={date} onChange={handleChangeDate} />
              </Form.Group>
              {errorTime && <Alert variant="danger" onClose={() => setErrorTime("")} dismissible>{errorTime}</Alert>}
              <Form.Group controlId="controlTime">
                <Form.Label>Time (Optional):</Form.Label>
                <Form.Control type="time" value={time} onChange={handleChangeTime} disabled={!date} />
              </Form.Group>
            </Modal.Body>
            <Modal.Footer>
              <Button variant="secondary" onClick={() => { history.goBack() }}>
                Close
          </Button>
              <Button variant="success" type="submit" onClick={handleSubmit}>
                {location.state ? "Update" : "Add"}
              </Button>
            </Modal.Footer>
          </Form>
        </Modal>}
    </>
  );
}

export default TasksAdd;
