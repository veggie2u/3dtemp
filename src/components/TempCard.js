import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import Avatar from '@material-ui/core/Avatar';
import Card from '@material-ui/core/Card';
import CardHeader from '@material-ui/core/CardHeader';
import CardActionArea from '@material-ui/core/CardActionArea';
import CardContent from '@material-ui/core/CardContent';
import CardMedia from '@material-ui/core/CardMedia';
import IconButton from '@material-ui/core/IconButton';
import Typography from '@material-ui/core/Typography';
import Box from '@material-ui/core/Box';
import { Timeline, NotificationImportant } from '@material-ui/icons';

const useStyles = makeStyles({
  root: {
    maxWidth: 345,
  },
  divider: {
    margin: `0 20px`
  },
  cardActionArea: {
    textAlign: 'center',
  }
});

const TempCard = () => {
  const classes = useStyles();

  return (
    <Card className={classes.root}>
      <CardHeader
        avatar={
          <Avatar aria-label="Sensor Id" className={classes.avatar}>
            03
          </Avatar>
        }
        action={
          <IconButton aria-label="Notification Important">
            <NotificationImportant color="secondary"/>
          </IconButton>
        }
        title="Filament Box 3"
        subheader="Recorded @ 2020/04/16 12:00"
      />
      <CardMedia
        component="img"
        alt="Sensor Location Image"
        height="160"
        image="/images/testImage.jpeg"
        title="Sensor Location"
      />
      <CardActionArea className={classes.cardActionArea}>
        <CardContent>
          <Box display="flex" alignItems="center">
            <Box flexGrow={1} justifyContent="flex-start">
              <Typography variant="h4" color="textSecondary" component="span">
                23.2
              </Typography>
              <Typography variant="h6" color="textSecondary" component="span">
                &nbsp;C
              </Typography>
            </Box>
            <Box flexGrow={2} justifyContent="center">
              <Timeline fontSize="large" color="primary" />
            </Box>
            <Box flexGrow={1} justifyContent="flex-end">
              <Typography variant="h4" color="textSecondary" component="span">
                40.0
              </Typography>
              <Typography variant="h6" color="textSecondary" component="span">
                &nbsp;Hpa
              </Typography>
            </Box>
          </Box>
        </CardContent>
      </CardActionArea>
      
    </Card>
  );
}

export default TempCard;
