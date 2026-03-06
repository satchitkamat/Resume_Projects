import React, { useEffect } from 'react';
import { useParams, Link, useNavigate } from 'react-router-dom';
import { projectsData } from '../data/projects';

const ProjectDetails = () => {
    const { id } = useParams();
    const navigate = useNavigate();
    const project = projectsData.find(p => p.id === id);

    useEffect(() => {
        window.scrollTo(0, 0);
    }, []);

    if (!project) {
        return (
            <div className="min-h-screen py-32 flex flex-col items-center justify-center">
                <h2 className="text-4xl font-bold text-white mb-4">Project Not Found</h2>
                <button
                    onClick={() => navigate('/')}
                    className="text-blue-400 hover:text-blue-300 transition-colors"
                >
                    &larr; Back to Portfolio
                </button>
            </div>
        );
    }

    return (
        <section className="py-32 min-h-screen relative">
            <div className="max-w-4xl mx-auto px-4 sm:px-6 lg:px-8">
                <button
                    onClick={() => navigate(-1)}
                    className="mb-8 inline-flex items-center text-blue-400 hover:text-blue-300 transition-colors font-medium"
                >
                    &larr; Back
                </button>

                <div className="glass rounded-3xl overflow-hidden shadow-[0_20px_60px_-15px_rgba(37,99,235,0.2)] border border-slate-700/50">
                    <div className="relative aspect-video bg-slate-800/50">
                        <img
                            src={project.image}
                            alt={project.title}
                            className="w-full h-full object-cover"
                        />
                    </div>

                    <div className="p-8 md:p-12">
                        <h1 className="text-3xl md:text-5xl font-bold text-white mb-6">
                            {project.title}
                        </h1>

                        <div className="flex flex-wrap gap-3 mb-8">
                            {project.tags.map((tag, index) => (
                                <span
                                    key={index}
                                    className="px-4 py-1.5 rounded-full bg-blue-900/40 text-blue-300 border border-blue-700/50 text-sm font-medium shadow-sm"
                                >
                                    {tag}
                                </span>
                            ))}
                        </div>

                        <div className="prose prose-invert max-w-none text-slate-300 mb-10 text-lg leading-relaxed space-y-8">
                            <div>
                                <h3 className="text-2xl font-bold text-white mb-4">Project Description</h3>
                                <p>{project.longDescription || project.description}</p>
                            </div>

                            {project.features && (
                                <div>
                                    <h3 className="text-2xl font-bold text-white mb-4">Key Features</h3>
                                    <ul className="list-disc pl-5 space-y-2">
                                        {project.features.map((feature, i) => (
                                            <li key={i}>{feature}</li>
                                        ))}
                                    </ul>
                                </div>
                            )}

                            {project.technologies && (
                                <div>
                                    <h3 className="text-2xl font-bold text-white mb-4">Technologies Used</h3>
                                    <div className="flex flex-wrap gap-2">
                                        {project.technologies.map((tech, i) => (
                                            <span key={i} className="px-3 py-1 bg-slate-800 text-blue-300 border border-slate-700 rounded-md text-sm">
                                                {tech}
                                            </span>
                                        ))}
                                    </div>
                                </div>
                            )}

                            {project.learnings && (
                                <div>
                                    <h3 className="text-2xl font-bold text-white mb-4">What I Learned</h3>
                                    <ul className="list-disc pl-5 space-y-2">
                                        {project.learnings.map((learning, i) => (
                                            <li key={i}>{learning}</li>
                                        ))}
                                    </ul>
                                </div>
                            )}

                            {project.creator && (
                                <div className="text-sm text-slate-400 italic mt-8 pt-4 border-t border-slate-700/50">
                                    Original Concept / Creator: {project.creator}
                                </div>
                            )}
                        </div>

                        <div className="flex flex-wrap gap-4 pt-4 mt-8">
                            {project.link !== '#' && (
                                <a
                                    href={project.link}
                                    target="_blank"
                                    rel="noopener noreferrer"
                                    className="inline-flex items-center justify-center px-6 py-3 rounded-xl bg-gradient-to-r from-blue-600 to-indigo-600 hover:from-blue-500 hover:to-indigo-500 text-white font-semibold transition-all shadow-lg hover:shadow-blue-500/25"
                                >
                                    Visit Live Site
                                    <svg className="w-5 h-5 ml-2" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M10 6H6a2 2 0 00-2 2v10a2 2 0 002 2h10a2 2 0 002-2v-4M14 4h6m0 0v6m0-6L10 14" />
                                    </svg>
                                </a>
                            )}
                        </div>
                    </div>
                </div>
            </div>
        </section>
    );
};

export default ProjectDetails;
