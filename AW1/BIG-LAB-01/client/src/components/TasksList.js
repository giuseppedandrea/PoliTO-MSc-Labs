import { ListGroup, Button } from 'react-bootstrap';
import { Link } from 'react-router-dom'
import { BsPersonSquare, BsPencil, BsTrash } from 'react-icons/bs';

function TasksList(props) {
  return (
    <div className="py-3">
      <h1>Filter: {props.selectedFilter.replace(/-/g, " ").replace(/\w\S*/g, (word) => (word.charAt(0).toUpperCase() + word.slice(1)))}</h1>
      <ListGroup variant="flush">
        {props.tasks.filter((task) => props.filters[props.selectedFilter](task)).map((task) => <TaskItem key={task.id} task={task} deleteTask={props.deleteTask} />)}
      </ListGroup>
    </div>
  );
}

function TaskItem(props) {
  return (
    <ListGroup.Item>
      <div className="d-flex w-100">
        <div className="w-50 align-self-center">
          <input className="form-check-inline" type="checkbox" value="" id={`ck${props.task.id}`} />
          <label className={!props.task.isUrgent ? "form-check-label" : "form-check-label urgent"} htmlFor={`ck${props.task.id}`}>{props.task.description}</label>
        </div>
        <div className="ml-3 align-self-center">
          {props.task.isPrivate && <BsPersonSquare title={"Private"} />}
        </div>
        <div className="d-flex ml-auto align-self-center">
          <div className="ml-3 align-self-center">
            {props.task.deadline && <span className="small">{props.task.deadline.format("ddd, DD-MM-YYYY HH:mm")}</span>}
          </div>
          <div className="ml-3 flex-shrink-0 align-self-center">
            <Link to={{ pathname: `/update/${props.task.id}`, state: props.task }}>
              <Button variant="light">
                <BsPencil title={"Edit"} />
              </Button>
            </Link>
            {" "}
            <Button variant="light" onClick={() => { props.deleteTask(props.task) }}>
              <BsTrash title={"Remove"} />
            </Button>
          </div>
        </div>
      </div>
    </ListGroup.Item>
  );
}

export default TasksList;
