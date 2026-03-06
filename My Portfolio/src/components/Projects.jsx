import React from 'react';

import { Link } from 'react-router-dom';
import { projectsData } from '../data/projects';

const Projects = () => {
    return (
        <section id="projects" className="py-24 relative">
            <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
                <div className="text-center mb-16">
                    <h2 className="text-3xl md:text-5xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-blue-400 to-indigo-500 mb-6">
                        Featured Projects
                    </h2>
                    <p className="text-slate-400 text-lg max-w-2xl mx-auto">
                        A selection of my best work in web development, focusing on design, performance, and user experience.
                    </p>
                </div>

                <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-8">
                    {projectsData.map((project, index) => (
                        <Link
                            to={`/project/${project.id}`}
                            key={index}
                            className="group rounded-2xl overflow-hidden glass hover:-translate-y-2 transition-all duration-300 hover:shadow-[0_10px_40px_-10px_rgba(37,99,235,0.3)] border border-slate-700/50 block"
                        >
                            <div className="h-56 overflow-hidden relative">
                                <div className="absolute inset-0 bg-blue-600/20 group-hover:bg-transparent transition-colors duration-300 z-10 mix-blend-overlay"></div>
                                <img
                                    src={project.image}
                                    alt={project.title}
                                    className="w-full h-full object-cover transform group-hover:scale-110 transition-transform duration-500"
                                />
                            </div>
                            <div className="p-8">
                                <h3 className="text-2xl font-bold text-white mb-3 group-hover:text-blue-400 transition-colors">{project.title}</h3>
                                <p className="text-slate-400 mb-6 leading-relaxed">{project.description}</p>
                                <div className="flex flex-wrap gap-2">
                                    {project.tags.map((tag, tagIndex) => (
                                        <span
                                            key={tagIndex}
                                            className="text-sm font-medium px-3 py-1 rounded-full bg-slate-800 text-blue-300 border border-blue-900/50 shadow-sm"
                                        >
                                            {tag}
                                        </span>
                                    ))}
                                </div>
                            </div>
                        </Link>
                    ))}
                </div>
            </div>
        </section>
    );
};

export default Projects;
