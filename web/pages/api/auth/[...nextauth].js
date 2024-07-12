"use strict";

import CredentialsProvider from "next-auth/providers/credentials";
import NextAuth from "next-auth";
import axios from "axios";

export default NextAuth({
	providers: [
		CredentialsProvider({
			name: "Credentials",
			credentials: {
				email: {
					label: "Email",
					type: "email",
					placeholder: "name@domain.com",
				},
				password: {
					label: "Password",
					type: "password",
					placeholder: "********",
				},
			},
			async authorize(credentials, request) {
				const data = await axios.post(`https://powervision.vercel.app/api/user/login`, {
					email: credentials.email,
					password: credentials.password,
				}).then((response) => {
					return response.data;
				}).catch((error) => {
					return null;
				});
				console.log(data);

				return data;
			},
		}),
	],
	callbacks: {
		jwt: async ({ token, user }) => {
			if (user) {
				token.acessToken = user.token;
				token.name = user.name;
				token.email = user.email;
				token.id = user.id;
				token.expires = new Date().getTime() + (86400000 * 5);
			}
			return token;
		},
		session: async ({ token, session }) => {
			if (token) {
				const now = new Date().getTime();
				if (token.expiresAt < now) {
					return null;
				}

				session.strategy = "database";
				session.maxAge = (86400 * 5);
				session.updateAge =	3600;
				session.expires = token.expires;
				session.acessToken = token.acessToken;
				session.user = {
					id: token.id,
					name: token.name,
					email: token.email,
				};
			}

			return session;
		}
	},
	secret: process.env.AUTH_SECRET,
	jwt: {
		secret: process.env.AUTH_SECRET,
		encryption: true,
	},
});
