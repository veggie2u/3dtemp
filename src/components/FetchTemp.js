import React, { useState, useEffect } from "react";
import { parseISO, format } from 'date-fns';
import config from "../aio_config.json";

const Fetch = () => {
  const [data, setData] = useState({ feeds: [] });
  const options = {
    method: 'GET',
    headers: {
      'Content-Type': 'application/json',
      'X-AIO-Key': config.aioKey
    }
  }
  const url = config.feedUrl;
  
  useEffect(() => {
    const fetchData = async () => {
      let res = await fetch(url, options);
      let response = await res.json();
      setData(response);
    };
    fetchData();
    // eslint-disable-next-line
  }, []);
  return (
    <table>
      <tbody>
      {data.feeds.map(feed => (
        <tr key={feed.key}>
          <td>{feed.name}</td><td>{feed.last_value}</td><td>{format(parseISO(feed.updated_at), 'MM/dd hh:mm:ss a')}</td>
        </tr>
      ))}
      </tbody>
    </table>
  );
};

export default Fetch;
