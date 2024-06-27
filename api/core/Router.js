"use strict";

const path = require("path");
const fs = require("fs");

module.exports = (API) => {
    fs.readdirSync(path.join(__dirname, "../routes"))
        .forEach((file) => require(path.resolve(__dirname, "../routes/" + file))(API));
};