/**
 * Copyright @ 2021 VW Group. All rights reserved.
 * 
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * 
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 * 
 * You may add additional accurate notices of copyright ownership.
 * 
 */


@Library(["cortex_jenkins_pipeline_lib", "cortex_jenkins_lib_settings"]) _

conan.settings = cortex_jenkins_lib_settings() {
    gitflow = true
    defaultUser = 'fep'
    conanVersion = '==1.28.2'
}
conan.settings.extraModules = ['wheel', 'sphinx', 'sphinx_rtd_theme', 'breathe==4.16.0'] // required for ARM build and documentation

def conanfileDir = "."

//skipIndexingBuilds()

pipeline {
    agent none

    triggers {
        cron(env.BRANCH_NAME == 'develop' ? 'H H(2-3) * * 1-5' : '')
    }

    options {
        skipDefaultCheckout()
    }

    stages {
        stage('Platform Lanes') {
            parallel {
                stage('Linux_x64_gcc5') {
                    agent { label "Linux_x64_gcc5 && AEV25" }
                    stages {
                        stage('Build Debug and Release') {
                            steps {
                                conanCreateUploadReleaseDebug(profile: 'Linux_x64_gcc5', conanFile: conanfileDir)
                            }
                            post {
                                always {
                                    conanPublishTestResults()
                                }
                            }
                        }
                    }
                    post {
                        always {
                            cleanWs()
                        }
                    }
                }
                stage('Linux_x64_gcc7 RelWithDebInfo') {
                    agent { label "Linux_x64_gcc7 && AEV25" }
                    stages {
                        stage('Build RelWithDebInfo') {
                            steps {
                                conanCreateUpload(profile: 'Linux_x64_gcc7', conanFile: conanfileDir)
                            }
                            post {
                                always {
                                    conanPublishTestResults()
                                }
                            }
                        }
                    }
                    post {
                        always {
                            cleanWs()
                        }
                    }
                }
                stage('Linux_x64_gcc7 Debug') {
                    agent { label "Linux_x64_gcc7 && AEV25" }
                    stages {
                        stage('Build Debug') {
                            steps {
                                conanCreateUpload(profile: 'Linux_x64_gcc7_dev', conanFile: conanfileDir)
                            }
                            post {
                                always {
                                    conanPublishTestResults()
                                }
                            }
                        }
                    }
                    post {
                        always {
                            cleanWs()
                        }
                    }
                }
                /*
                stage('Linux_armv8_gcc5') {
                    agent { label "Linux_armv8_gcc5" }
                    stages {
                        stage('Build Debug and Release') {
                            steps {
                                conanCreateUploadReleaseDebug(profile: 'Linux_armv8_gcc5', conanFile: conanfileDir)
                            }
                            post {
                                always {
                                    conanPublishTestResults()
                                }
                            }
                        }
                    }
                    post {
                        always {
                            cleanWs()
                        }
                    }
                }*/
                stage('Windows_x64_vc141_VS2017') {
                    agent { label "Windows_x64_vc141_VS2017 && AEV25" }
                    stages {
                        stage('Build Debug and Release') {
                            steps {
                                conanCreateUploadReleaseDebug(profile: 'Windows_x64_vc141_VS2017', conanFile: conanfileDir)
                            }
                            post {
                                always {
                                    conanPublishTestResults()
                                }
                            }
                        }
                    }
                    post {
                        always {
                            cleanWs()
                        }
                    }
                }
            }
        }
    }
}
