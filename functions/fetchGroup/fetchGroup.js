import fetch from "node-fetch";

/**
 * Netlify function that returns feeds from a group from AIO
 */
const { AIO_KEY, AIO_URL } = process.env;

exports.handler = async (event, context) => {
  return fetch(AIO_URL, { headers: { "Accept": "application/json", 'X-AIO-Key': AIO_KEY } })
    .then(response => response.json())
    .then(data => ({
      statusCode: 200,
      body: data
    }))
    .catch(error => ({ statusCode: 500, body: String(error) }));
};