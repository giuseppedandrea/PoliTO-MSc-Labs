import { Container, Row, Col, Button, Form, Alert } from 'react-bootstrap';
import { useState } from 'react';

function LoginForm(props) {
  const [email, setEmail] = useState("john.doe@polito.it");
  const [password, setPassword] = useState("password");
  const [errorEmail, setErrorEmail] = useState("");
  const [errorPassword, setErrorPassword] = useState("");

  const handleChangeEmail = (event) => {
    setErrorEmail("");
    setEmail(event.target.value);
  }

  const handleChangePassword = (event) => {
    setErrorPassword("");
    setPassword(event.target.value);
  }

  const handleSubmit = (event) => {
    event.preventDefault();

    let validForm = true;
    if (email === "") {
      setErrorEmail("Empty E-Mail field");
      validForm = false;
    } else if (!email.match(/^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*$/)) {
      setErrorEmail("Invalid E-Mail field");
      validForm = false;
    }
    if (password === "") {
      setErrorPassword("Empty Password field");
      validForm = false;
    }

    if (validForm) {
      props.doLogin({ username: email, password: password });
    }
  };

  return (
    <>
      <Container fluid>
        <Row className="justify-content-md-center min-vh-100">
          <Col md={4} className="pt-nav my-auto">
            {/* General Alert */}
            {props.message && <Alert className="my-3" variant={props.message.type} onClose={() => props.setMessage("")} dismissible>{props.message.msg}</Alert>}
            {/* Form */}
            <Form>
              {/* E-Mail field */}
              <Form.Group controlId="email">
                <Form.Label>E-Mail</Form.Label>
                <Form.Control type="email" value={email} isInvalid={errorEmail} onChange={handleChangeEmail} />
                <Form.Control.Feedback type="invalid">{errorEmail}</Form.Control.Feedback>
              </Form.Group>
              {/* Password field */}
              <Form.Group controlId="password">
                <Form.Label>Password</Form.Label>
                <Form.Control type="password" value={password}  isInvalid={errorPassword} onChange={handleChangePassword} />
                <Form.Control.Feedback type="invalid">{errorPassword}</Form.Control.Feedback>
              </Form.Group>
              {/* Submit Button */}
              <Button variant="success" onClick={handleSubmit}>Login</Button>
            </Form>
          </Col>
        </Row>
      </Container>
    </>
  );
}

export default LoginForm;
