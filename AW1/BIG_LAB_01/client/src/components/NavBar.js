import { Navbar, Nav, Form, FormControl } from 'react-bootstrap';
import { BsCheckAll, BsPeopleCircle } from 'react-icons/bs';

function NavBar(props) {
  return (
    <Navbar bg="success" variant="dark" expand="md" fixed="top">
      {/* Toggle sidebar */}
      <Navbar.Toggle aria-controls="sidebar" onClick={() => { props.toggleCollapse() }} />
      {/* Logo and title */}
      <Navbar.Brand href="/">
        <BsCheckAll title={"Logo"} size={"30px"}></BsCheckAll>{" "}
          ToDO Manager
        </Navbar.Brand>
      {/* Search form */}
      <Form inline className="mx-auto d-none d-md-block">
        <FormControl type="text" placeholder="Search" className="mr-md-2" />
      </Form>
      {/* Profile button */}
      <Nav className="mr-md-2">
        <Nav.Link href="#">
          <BsPeopleCircle title={"User"} size={"30px"}></BsPeopleCircle>
        </Nav.Link>
      </Nav>
    </Navbar>
  );
}

export default NavBar;
