import type {Dialect, Model} from "sequelize";
import {Sequelize} from "sequelize";
import {Users} from "./Users";
import {Roles} from "./Roles";
import {hashPassword, verifyPassword} from "../Util";
import {Tasks} from "./Tasks";


export class DataBase {
  public sequelize: Sequelize;
  private users: Users;
  private roles: Roles;
  private tasks: Tasks;


  constructor(
    nameBD: string,
    nameUser: string,
    password: string,
    dialect: Dialect = "mysql",
    host = "localhost",
    port = 3306
  ) {
    this.sequelize = new Sequelize(nameBD, nameUser, password, {
      dialect,
      host,
      port,
    });
    this.users = new Users(this.sequelize);
    this.roles = new Roles(this.sequelize);
    this.tasks = new Tasks(this.sequelize);

    this.users.users.hasMany( this.roles.roles, {
      foreignKey: "Role_id",
      as: "RolesID",
    });
    this.roles.roles.belongsTo(this.users.users, {
      foreignKey: "Role_id",
      as: "RolesID",
    });
    this.users.users.belongsTo( this.tasks.tasks, {
      foreignKey: "User_id",
      as: "UserTaskID",
    });
    this.tasks.tasks.hasMany(this.users.users, {
      foreignKey: "User_id",
      as: "UserTaskID",
    });

  }

  public getAllUser(callback: any) {
    this.users
      .getAllUser()
      .then((result: Model[]) => {
        const users: any = [];
        if (result) {
          result.forEach((element, index, array) => {
            users.push(array[index].dataValues);
          });
        }
        callback(result);
      })
      .catch(() => {
        callback([]);
      });
  }

  public getUserByEmail(email: string, callback: any) {
    this.users
      .getUserByEmail(email)
      .then((result: Model) => {
        let user;
        if (result) {
          user = result.dataValues;
        }
        callback(user);
      })
      .catch((e:any) => {
        console.log(e);
        callback(null);
      });
  }

  public getUserById(id: string, callback: any) {
    this.users
      .getUserById(id)
      .then((result: Model) => {
        let user;
        if (result) {
          user = result.dataValues;
        }
        callback(user);
      })
      .catch((e: any) => {
        callback({e});
      });
  }

  public addUser(user: any, callback: any) {
    this.users
      .addUser(user)
      .then((result: Model) => {
        let userResult;
        if (result) {
          userResult = result.dataValues;
        }
        callback(userResult);
      })
      .catch(() => {
        callback(null);
      });
  }

  public updateUser(userData: any, callback: any) {
    this.users
      .getUserByEmail(userData.userEmailOld)
      .then((result: any) => {
        if (result) {
          result.update({
            Email: userData.userEmailNew,
            Password: userData.userPassword
              ? hashPassword(userData.userPassword)
              : result.Password,
            Role: userData.Role ? userData.Role : result.Role,
            Name: userData.userName,
            LastName: userData.userLastName,
            Src: userData.src,
          });
          result
            .save()
            .then((res: any) => {
              callback(res);
            })
            .catch((e: any) => {
              callback({error: e});
            });
        }
      })
      .catch((e: any) => {
        callback({error: e});
      });
  }

  public checkPassword(passwordOne: string, passwordTwo: string) {
    return verifyPassword(passwordOne, passwordTwo);
  }

  public addTask(task: any, callback: any) {
    this.tasks
      .addTask(task)
      .then((result: Model) => {
        let userResult;
        if (result) {
          userResult = result.dataValues;
        }
        callback(userResult);
      })
      .catch((e:any) => {
        callback(e);
      });
  }

  public getAllTask(user_id:string,callback: any) {
    this.tasks
      .getAllTask(user_id)
      .then((result: Model[]) => {
        const tasks: any = [];
        if (result) {
          result.forEach((element, index, array) => {
            let task = element.dataValues
            task["task_id"] = task["Task_id"]
            task["text"] = task["Text"]
            task["user_id"] = task["User_id"]
            task["state"] = task["State"]
            delete task["Task_id"]
            delete task["Text"]
            delete task["User_id"]
            delete task["State"]
            tasks.push(task);
          });
        }
        callback(tasks);
      })
      .catch(() => {
        callback([]);
      });
  }
}
