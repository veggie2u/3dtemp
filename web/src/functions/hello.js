/**
 * Netlify function that returns Hello World or a name parameter
 */
exports.handler = async (event, context) => {
  const name = event.queryStringParameters.name || "World";

  return {
    statusCode: 200,
    body: `Hello there, ${name}`
  };
};
