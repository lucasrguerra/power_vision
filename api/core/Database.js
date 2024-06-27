"use strict";

const mongoose = require("mongoose");
require("dotenv").config();

const USERNAME = process.env.DATABASE_USERNAME;
const PASSWORD = process.env.DATABASE_PASSWORD;
const HOST = process.env.DATABASE_HOST;

mongoose.connect(`mongodb+srv://${USERNAME}:${PASSWORD}@${HOST}`);
mongoose.Promise = global.Promise;

const Database = mongoose;
module.exports = Database;