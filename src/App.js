import React from "react";
import NavBar from "./components/NavBar";
import { Router, Route, Switch } from "react-router-dom";
import { CssBaseline } from '@material-ui/core';
import { Container } from '@material-ui/core';
import { makeStyles } from '@material-ui/core/styles';
import history from "./utils/history";
import PrivateRoute from "./components/PrivateRoute";
import Profile from "./components/Profile";
import Fetch from "./components/FetchTemp";
import Splash from "./components/Splash";

const useStyles = makeStyles({
  root: {
    paddingTop: '25px',
  },
});

function App() {
  const classes = useStyles();
  return (
    <div className="App">
      <Router history={history}>
        <CssBaseline />
        <header>
          <NavBar />
        </header>
        <Container maxWidth="false" className={classes.root}>
          <Switch>
            <Route path="/" exact component={Splash} />
            <PrivateRoute path="/profile" component={Profile} />
            <PrivateRoute path="/fetch" component={Fetch} />
          </Switch>
        </Container>
      </Router>
    </div>
  );
}

export default App;
