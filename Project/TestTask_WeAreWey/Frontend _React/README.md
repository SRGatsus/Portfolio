docker build -t TestTask_React
docker run -d -p 8080:3000 --rm --name TestTask_React TestTask_React