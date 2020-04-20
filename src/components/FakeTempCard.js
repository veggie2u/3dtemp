import React from 'react';
import TempCard from './TempCard';

const feed = {
  id: "02",
  name: "Humidity 1",
  last_value: "23.0",
  created_at: "2020-04-7T01:08:16Z"
};

const FakeTempCard = () => {
  const props = { feed };
  return (
    <TempCard feed={props.feed} />
  );
}

export default FakeTempCard;
