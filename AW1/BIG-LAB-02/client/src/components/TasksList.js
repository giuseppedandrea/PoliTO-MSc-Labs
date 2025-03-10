import { ListGroup, Button } from 'react-bootstrap';
import { Link } from 'react-router-dom';
import { BsPersonSquare, BsPencil, BsTrash } from 'react-icons/bs';

function TasksList(props) {
  return (
    <div className="py-3">
      <h1>Filter: {props.selectedFilter.replace(/-/g, " ").replace(/\w\S*/g, (word) => (word.charAt(0).toUpperCase() + word.slice(1)))}</h1>
      {
        props.loading
          ? <span>🕗 Please wait, loading your tasks... 🕗</span>
          : !props.tasks.length
            ? <span>No tasks</span>
            : <ListGroup variant="flush">{props.tasks.map((task) => <TaskItem key={task.id} task={task} completeTask={props.completeTask} deleteTask={props.deleteTask} />)}</ListGroup>
      }
    </div>
  );
}

function TaskItem(props) {
  let statusVariant = undefined;
  switch (props.task.status) {
    case "added":
      statusVariant = "success";
      break;
    case "deleted":
      statusVariant = "danger";
      break;
    case "updated":
    case "completed":
      statusVariant = "warning";
      break;
    default:
      break;
  }

  const handleChangeIsCompleted = () => {
    const task = { id: props.task.id, description: props.task.description, isImportant: props.task.isImportant, isPrivate: props.task.isPrivate, isCompleted: !props.task.isCompleted, deadline: props.task.deadline, user: props.task.user };
    props.completeTask(task);
  }

  return (
    <ListGroup.Item variant={statusVariant}>
      <div className="d-flex w-100">
        <div className="w-50 align-self-center">
          <input className="form-check-inline" type="checkbox" value="" id={`ck${props.task.id}`} checked={props.task.isCompleted} onChange={handleChangeIsCompleted} />
          <label className={!props.task.isImportant ? "form-check-label" : "form-check-label important"} htmlFor={`ck${props.task.id}`}>{props.task.description}</label>
        </div>
        <div className="ml-3 align-self-center">
          {!props.task.isPrivate && <BsPersonSquare title={"Public"} />}
        </div>
        <div className="d-flex ml-auto align-self-center">
          <div className="ml-3 align-self-center">
            {props.task.deadline && <span className="small">{props.task.deadline.format("ddd, DD-MM-YYYY HH:mm")}</span>}
          </div>
          <div className="ml-3 flex-shrink-0 align-self-center">
            <Link to={{ pathname: `/update/${props.task.id}`, state: { id: props.task.id, description: props.task.description, isImportant: props.task.isImportant, isPrivate: props.task.isPrivate, isCompleted: props.task.isCompleted, deadline: props.task.deadline, user: props.task.user } }}>
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
