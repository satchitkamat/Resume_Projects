import React from 'react';

const Hero = () => {
    return (
        <section id="home" className="relative h-screen flex items-center justify-center pt-20 overflow-hidden">
            {/* Background gradients */}
            <div className="absolute top-0 left-0 w-full h-full overflow-hidden -z-10">
                <div className="absolute top-[-10%] left-[-10%] w-96 h-96 rounded-full bg-blue-600/20 blur-[100px]" />
                <div className="absolute bottom-[-10%] right-[-5%] w-96 h-96 rounded-full bg-indigo-600/20 blur-[100px]" />
            </div>

            <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 flex flex-col-reverse md:flex-row items-center justify-between gap-12 w-full">
                <div className="flex-1 text-center md:text-left">
                    <h1 className="text-5xl md:text-7xl font-extrabold tracking-tight mb-6">
                        <span className="block text-slate-100 mb-2">Hi, I'm</span>
                        <span className="block bg-clip-text text-transparent bg-gradient-to-r from-blue-400 to-indigo-500 pb-3">
                            Satchit Kamat
                        </span>
                    </h1>
                    <p className="mt-6 text-xl text-slate-400 mb-10 leading-relaxed max-w-2xl">
                        I'm currently in my first year of BCA. I'm passionate about technology, programming, and building engaging digital experiences. Let's create something amazing together.
                    </p>
                    <div className="flex flex-col sm:flex-row justify-center md:justify-start gap-4">
                        <a
                            href="#projects"
                            className="px-8 py-4 rounded-full bg-blue-600 hover:bg-blue-700 text-white font-semibold transition-all shadow-[0_0_20px_rgba(37,99,235,0.4)] hover:shadow-[0_0_30px_rgba(37,99,235,0.6)]"
                        >
                            View My Work
                        </a>
                        <a
                            href="#contact"
                            className="px-8 py-4 rounded-full bg-slate-800 hover:bg-slate-700 text-white font-semibold border border-slate-700 transition-all hover:border-slate-500"
                        >
                            Contact Me
                        </a>
                    </div>
                </div>

                <div className="flex-1 flex justify-center md:justify-end mb-8 md:mb-0">
                    <div className="relative w-64 h-64 md:w-96 md:h-96">
                        <div className="absolute inset-0 rounded-full bg-gradient-to-tr from-blue-500 to-indigo-500 blur-2xl opacity-40 animate-pulse"></div>
                        <div className="relative w-full h-full rounded-full border-4 border-slate-800 overflow-hidden glass shadow-2xl">
                            <img
                                src="/profile.jpg"
                                alt="Satchit Vidyadhar Kamat"
                                className="w-full h-full object-cover relative z-10"
                            />
                        </div>
                    </div>
                </div>
            </div>
        </section>
    );
};

export default Hero;
