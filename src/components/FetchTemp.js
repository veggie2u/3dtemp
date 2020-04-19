import React, { useState, useEffect } from "react";
import { parseISO, format } from 'date-fns';
import TempCard from "./TempCard";

const Fetch = () => {
  const [data, setData] = useState({ feeds: [] });

  const url = '/api/fetchGroup';

  useEffect(() => {
    const fetchData = async () => {
      let res = await fetch(url);
      let response = await res.json();
      setData(response);
    };
    fetchData();
    // eslint-disable-next-line
  }, []);
  return (
    <React.Fragment>
      {data.feeds.map(feed => (
        <TempCard />
      ))}
    
      <table>
        <tbody>
        {data.feeds.map(feed => (
          <tr key={feed.key}>
            <td>{feed.name}</td><td>{feed.last_value}</td><td>{format(parseISO(feed.updated_at), 'MM/dd hh:mm:ss a')}</td>
          </tr>
        ))}
        </tbody>
      </table>
    </React.Fragment>
  );
};

export default Fetch;
