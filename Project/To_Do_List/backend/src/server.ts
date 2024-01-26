import express from "express"
import bodyParser from "body-parser"
import {DataBase} from "./DataBase";
import * as jwt from "jsonwebtoken";
import {verifyPassword} from "./Util";

const app = express();
const dataBase = new DataBase("to_do_list", "root", "", "mysql", "localhost", 3306);
const SECRET_KET = "TO_DO_LIST_GATSUS";

function generateToken(id: any, role: any): string {
  const payload = {
    id,
    role,
  };
  return jwt.sign(payload, SECRET_KET, {expiresIn: "24h"});
}

app.use(bodyParser.json())
app.use(bodyParser.urlencoded({extended: false}))

app.post('/auth', function (req, res) {
  let data = req.body;
  let body;
  dataBase.getUserByEmail(data.email, function (resUserByEmail: any) {
    if (resUserByEmail && verifyPassword(data.password, resUserByEmail.Password) && verifyPassword(data.secretKey, resUserByEmail.SecretKey)) {
      body = {
        token: generateToken(resUserByEmail.User_id, resUserByEmail.Role_id),
        error: null,
      }
      res.send(JSON.stringify(body))
      return;
    } else {
      body = {
        error: "Ошибка авторизации",
        token: ""
      }
      res.send(JSON.stringify(body))
      return;
      /*dataBase.addUser(data, function (resCreateUser: any) {
        if (resCreateUser) {
          body = {
            token: generateToken(resCreateUser.User_id, resCreateUser.Role_id),
            error: null,
          }
        } else {
          body = {
            error: "Ошибка авторизации",
            token:""
          }
          res.send(JSON.stringify(body))
          return;
        }
      })*/
    }
  })
})

app.get('/getlist', function (req, res) {

  let authorization = req.headers.authorization;
  if (authorization) {
    let tokens = authorization.split(" ");
    if (tokens.length > 1) {
      let token = tokens[1];
      const dataToken: any = jwt.verify(token, SECRET_KET);
      if (dataToken.id !== "" && dataToken.role !== "") {
        dataBase.getAllTask(dataToken.id,function (resTask: any) {
          res.send(JSON.stringify(resTask));
        })
        return;
      }
    }
  }
  res.send('no');
  return;
})

app.post('/setlist', function (req, res) {


  res.send('Hello World')
})

app.post('/add', function (req, res) {
  let authorization = req.headers.authorization;
  if (authorization) {
    let tokens = authorization.split(" ");
    if (tokens.length > 1) {
      let token = tokens[1];
      const dataToken: any = jwt.verify(token, SECRET_KET);
      if (dataToken.id !== "" && dataToken.role !== "") {
        let task = req.body
        task["user_id"] = dataToken.id
        dataBase.addTask(task, function (resTask: any) {
          if (resTask) {
            res.send();
          }
        })
        return;
      }
    }
  }
  res.send('no');
})

app.get('/check', function (req, res) {
  let authorization = req.headers.authorization;
  if (authorization) {
    let tokens = authorization.split(" ");
    if (tokens.length > 1) {
      let token = tokens[1];
      const dataToken: any = jwt.verify(token, SECRET_KET);
      if (dataToken.id !== "" && dataToken.role !== "") {
        res.send('yes');
        return;
      }
    }
  }
  res.send('no');
  return;
})

app.listen(3000)