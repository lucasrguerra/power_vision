"use strict";

import { useSession } from "next-auth/react";
import Image from "next/image";

import Header from "/components/Header";
import Footer from "/components/Footer";
import copySpan from "/components/copySpan";
import Logo from "/public/logo.png";

export default function page() {
	const { data: session } = useSession();

	return (
		<>
			<Header session={session}/>

			{session ? (
				<div className="mx-4">
					<div className="flex justify-center py-8">
						<Image src={Logo} alt="Logo" width={300} height={300} />
					</div>

					<div className="text-center my-8">
						<div className="md:w-8/12 mx-auto">
							<p className="text-3xl font-bold">
								Welcome, {session.user.name}!
							</p>
							<p className="text-xl font-normal">
								Here you can monitor and analyze your energy consumption in real time with your devices.
								To connect your devices, use the following token: {copySpan(session.user.id)}
							</p>
						</div>
					</div>
				</div>
			) : (
				<div className="mx-4">
					<div className="flex justify-center py-8">
						<Image src={Logo} alt="Logo" width={300} height={300} />
					</div>

					<div className="md:w-8/12 mx-auto">
						<h1 className="text-4xl text-center">Welcome to Power Vision</h1>
						<p className="text-center py-8">
							Power Vision is an open source project aimed at developing a real-time electrical power quality analyzer using
							low-cost hardware with the ESP32-S3 microcontroller. This analyzer is capable of measuring and calculating a
							variety of electrical parameters in single-phase and three-phase systems, providing crucial information for
							analyzing and monitoring energy consumption.
						</p>
						<p className="text-center pb-8">
							The project was developed as a Course Completion Work (TCC) for the Computer Science course at the Federal
							Rural University of Pernmabuco (UFRPE) and guided by Prof. The project's main objective is to implement a
							low-cost, high-precision electrical energy analyzer that can be used in homes, companies and industries to
							monitor and analyze energy consumption.
						</p>
					</div>
				</div>
			)}

			<Footer />
		</>
	);
}