"use strict";

const formidable = require("express-formidable");
const express = require("express");
const helmet = require("helmet");
const cors = require("cors");
const path = require("path");
require("dotenv").config();

const API = express();

API.use(cors());
API.use(helmet());
API.use(
	formidable({
		encoding: "utf-8",
		multiples: true,
		keepExtensions: true,
		uploadDir: path.join(__dirname, "uploads"),
	})
);
API.use("/docs", (request, response, next) => {
	response.setHeader("Cache-Control", "no-store");
	next();
});
API.use("/docs", express.static(path.join(__dirname, "docs")));

require("./core/Router")(API);

const PORT = process.env.PORT || 3000;
API.listen(PORT, () => {
	console.log(`Server running on port ${PORT}`);
});

module.exports = API;