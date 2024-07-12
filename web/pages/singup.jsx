"use strict";

import { passwordStrength } from "check-password-strength";
import { useSession, signIn } from "next-auth/react";
import { useState } from "react";
import axios from "axios";

import Header from "/components/Header";
import Footer from "/components/Footer";

import TextField from '@mui/material/TextField';
import Button from '@mui/material/Button';

export default function page() {
	const { data: session } = useSession();

	const [name, setName] = useState("");
	const [email, setEmail] = useState("");
	const [password, setPassword] = useState("");
	const [passwordConfirmation, setPasswordConfirmation] = useState("");

	const [nameError, setNameError] = useState("");
	const [emailError, setEmailError] = useState("");
	const [passwordError, setPasswordError] = useState("");
	const [passwordStrengthId, setPasswordStrengthId] = useState(-1);
	const [passwordStrengthColor, setPasswordStrengthColor] = useState("");
	const [passwordConfirmationError, setPasswordConfirmationError] = useState("");
	const [passwordConfirmationStrengthId, setPasswordConfirmationStrengthId] = useState(-1);
	const [passwordConfirmationStrengthColor, setPasswordConfirmationStrengthColor] = useState("");

	const [loading, setLoading] = useState(false);

	function validateEmail(email) {
		return email.match(/^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/);
	};

	return (
		<>
			<Header session={session}/>

			<main className="">
				<div className="text-center text-2xl font-semibold my-8">
					Create your account
				</div>

				<div className="text-center mx-8 md:mx-auto md:w-8/12">
					<TextField
						required={true}
						label="Name"
						type="text"
						variant="outlined"
						className="w-full mb-2"
						value={name}
						onChange={(event => {
							setName(event.target.value);
							setNameError("");
							setLoading(false);
						})}
						error={nameError ? true : false}
						helperText={nameError}
					/>

					<TextField
						required={true}
						label="Email"
						type="email"
						variant="outlined"
						className="w-full my-2"
						value={email}
						onChange={(event => {
							setEmail(event.target.value);
							setEmailError("");
							setLoading(false);
						})}
						error={emailError ? true : false}
						helperText={emailError}
					/>

					<TextField
						required={true}
						label="Password"
						type="password"
						variant="outlined"
						className="w-full my-2"
						value={password}
						onChange={(event => {
							setPassword(event.target.value);
							setPasswordError("");
							setLoading(false);
							let thisPasswordStrengthId = passwordStrength(event.target.value).id;
							setPasswordStrengthId(thisPasswordStrengthId);
							if (thisPasswordStrengthId == 0) { setPasswordStrengthColor("bg-red-500"); }
							if (thisPasswordStrengthId == 1) { setPasswordStrengthColor("bg-orange-500"); }
							if (thisPasswordStrengthId == 2) { setPasswordStrengthColor("bg-green-500"); }
							if (thisPasswordStrengthId == 3) { setPasswordStrengthColor("bg-blue-500"); }
						})}
						error={passwordError ? true : false}
						helperText={passwordError}
					/>
					<div className="flex gap-2">
						<p className={`h-2 w-full rounded-full ${passwordStrengthId >= 0 ? passwordStrengthColor : "bg-gray-200"}`}></p>
						<p className={`h-2 w-full rounded-full ${passwordStrengthId > 0 ? passwordStrengthColor : "bg-gray-200"}`}></p>
						<p className={`h-2 w-full rounded-full ${passwordStrengthId > 1 ? passwordStrengthColor : "bg-gray-200"}`}></p>
						<p className={`h-2 w-full rounded-full ${passwordStrengthId > 2 ? passwordStrengthColor : "bg-gray-200"}`}></p>
					</div>

					<TextField
						required={true}
						label="Password Confirmation"
						type="password"
						variant="outlined"
						className="w-full my-2"
						value={passwordConfirmation}
						onChange={(event => {
							setPasswordConfirmation(event.target.value);
							setPasswordConfirmationError("");
							setLoading(false);
							let thisPasswordConfirmationStrengthId = passwordStrength(event.target.value).id;
							setPasswordConfirmationStrengthId(thisPasswordConfirmationStrengthId);
							if (thisPasswordConfirmationStrengthId == 0) { setPasswordConfirmationStrengthColor("bg-red-500"); }
							if (thisPasswordConfirmationStrengthId == 1) { setPasswordConfirmationStrengthColor("bg-orange-500"); }
							if (thisPasswordConfirmationStrengthId == 2) { setPasswordConfirmationStrengthColor("bg-green-500"); }
							if (thisPasswordConfirmationStrengthId == 3) { setPasswordConfirmationStrengthColor("bg-blue-500"); }
						})}
						error={passwordConfirmationError ? true : false}
						helperText={passwordConfirmationError}
					/>
					<div className="flex gap-2">
						<p className={`h-2 w-full rounded-full ${passwordConfirmationStrengthId >= 0 ? passwordConfirmationStrengthColor : "bg-gray-200"}`}></p>
						<p className={`h-2 w-full rounded-full ${passwordConfirmationStrengthId > 0 ? passwordConfirmationStrengthColor : "bg-gray-200"}`}></p>
						<p className={`h-2 w-full rounded-full ${passwordConfirmationStrengthId > 1 ? passwordConfirmationStrengthColor : "bg-gray-200"}`}></p>
						<p className={`h-2 w-full rounded-full ${passwordConfirmationStrengthId > 2 ? passwordConfirmationStrengthColor : "bg-gray-200"}`}></p>
					</div>

					<Button
					 variant="contained"
					 color={loading ? "info" : "success"}
					 className="my-4 w-8/12"
					 onClick={async (event) => {
						event.preventDefault();
						setLoading(true);

						if (name === "") { setNameError("Name is required"); return; }
						if (email === "") { setEmailError("Email is required"); return; }
						if (password === "") { setPasswordError("Password is required"); return; }
						if (passwordConfirmation === "") { setPasswordConfirmationError("Password Confirmation is required"); return; }
						if (password !== passwordConfirmation) { setPasswordConfirmationError("Password and Password Confirmation must be the same"); return; }

						if (!validateEmail(email)) { setEmailError("Invalid email"); return; }
						if (passwordStrength(password).id < 2) { setPasswordError("Password is weak"); return; }

						await axios.post("/api/user/create", {
							name: name,
							email: email,
							password: password,
						}).then((response) => {
							console.log(response);
							alert("Account created");
							signIn();
						}).catch((error) => {
							console.log(error);
							alert(error.response.data.error);
						});

					 }}
					>
						Sign Up
					</Button>
				</div>
			</main>
		</>
	);
}