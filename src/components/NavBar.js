import React from "react";
import { useAuth0 } from "../react-auth0-spa";
import { Link } from "react-router-dom";

import { makeStyles } from '@material-ui/core/styles';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import Button from '@material-ui/core/Button';
import IconButton from '@material-ui/core/IconButton';
import MenuIcon from '@material-ui/icons/Menu';

const useStyles = makeStyles((theme) => ({
  root: {
    flexGrow: 1,
  },
  menuButton: {
    marginRight: theme.spacing(2),
  },
  title: {
    flexGrow: 1,
  },
}));

const NavBar = () => {
  const { isAuthenticated, loginWithRedirect, logout } = useAuth0();
  const classes = useStyles();
  return (
    <AppBar position="static">
      <Toolbar>
        <IconButton edge="start" className={classes.menuButton} color="inherit" aria-label="menu">
          <MenuIcon />
        </IconButton>
        <Typography variant="h6" className={classes.title}>
          3D Temp
        </Typography>
        {isAuthenticated && (<React.Fragment>
          <Button color="inherit"><Link to="/">Home</Link></Button>
          <Button color="inherit"><Link to="/profile">Profile</Link></Button>
          <Button color="inherit"><Link to="/fetch">Fetch</Link></Button>
        </React.Fragment>)}
        {!isAuthenticated && (
          <Button color="inherit" onClick={() => loginWithRedirect({})}>Login</Button>
        )}
        {isAuthenticated && <Button color="inherit" onClick={() => logout()}>Logout</Button>}
      </Toolbar>
    </AppBar>
  );
};

export default NavBar;
