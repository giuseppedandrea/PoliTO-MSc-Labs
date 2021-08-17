import { Col, ListGroup, Collapse } from 'react-bootstrap';
import { Link } from 'react-router-dom';

function Sidebar(props) {
  return (
    <Collapse in={props.openCollapse}>
      <Col md={4} className="d-md-block bg-light pt-nav" id="sidebar">
        <ListGroup variant="flush" className="py-3">
          {props.filters.map((filter, idx) =>
            <FilterItem key={idx} filter={filter} selectedFilter={props.selectedFilter} changeFilter={props.changeFilter} />
          )}
        </ListGroup>
      </Col>
    </Collapse>
  );
}

function FilterItem(props) {
  return (
    <Link className="text-link" onClick={() => props.changeFilter(props.filter)} to={`/tasks/${props.filter}`}>
      <ListGroup.Item action active={props.filter === props.selectedFilter}>
        {props.filter.replace(/-/g, " ").replace(/\w\S*/g, (word) => (word.charAt(0).toUpperCase() + word.slice(1)))}
      </ListGroup.Item>
    </Link>
  );
}

export default Sidebar;
