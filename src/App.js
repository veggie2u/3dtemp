import React from "react";
import NavBar from "./components/NavBar";
import { Router, Route, Switch } from "react-router-dom"
import CssBaseline from '@material-ui/core/CssBaseline';

import history from "./utils/history";
import PrivateRoute from "./components/PrivateRoute";
import Profile from "./components/Profile";
import Fetch from "./components/FetchTemp";
import Splash from "./components/Splash";

function App() {
  return (
    <div className="App">
      <Router history={history}>
        <CssBaseline />
        <header>
          <NavBar />
        </header>
        <Switch>
          <Route path="/" exact component={Splash} />
          <PrivateRoute path="/profile" component={Profile} />
          <PrivateRoute path="/fetch" component={Fetch} />
        </Switch>
      </Router>
    </div>
  );
}

export default App;
