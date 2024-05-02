[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- ABOUT THE PROJECT -->
## About The Project


The File Transfer protocol is thought for transferring files between a client and a server in order to store or
retrieve them from the server. This protocol bases on the TCP transport protocol and uses the well-known ports
20 and 21. Two TCP connections are involved: The control connection (port 21) and the data connection (port
20). The control connection is used to transfer commands and the replies to these commands. The transfer of a
file needs to use several commands. Note that here the word “command” does not refer to the commands that
the user types into the command line of the FTP client program, it refers to the textual commands, defined in
the FTP protocol, that the client will send to the server.

Two different file transfer modes can be distinguished:

• Active mode: The client connects from a unprivileged port N to the server command port (port 21).
Then, the client starts listening to port M and sends the FTP command PORT M to the FTP server. The
server will then connect back from port 20 to port M of the client. Figure 1 illustrates the active mode.
The main problem with this procedure is that the server connects back to the client, which often is a
problem with firewalls that drop non known incoming connections.

• Passive mode: To solve the issue that arises when using active mode, the clients support passive mode.
In passive mode, the client initiates both connections (control and data).
When opening an FTP connection, the client opens two random unprivileged ports locally. The first port
contacts the server on port 21. Then the client issues a PASV command and the server responds with a
port number. After that the client connects to that port in order to transfer the data.

This repository contains a robust implementation of a File Transfer Protocol (FTP) server using sockets. With support for both active and passive mode connections, this server facilitates seamless file transfer between clients and the server.

Whether you're building a standalone FTP server or integrating FTP functionality into an existing application, this repository provides a solid foundation for your file transfer needs.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With


* ![C++][C++.js]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

None

### Installation

No installation required :)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

1. Download the files
2. Compile with g++
3. All ready!

<p align="right">(<a href="#readme-top">back to top</a>)</p>




<!-- LICENSE -->
## License

Distributed under the MIT License.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Joel Óscar - josc.margut@gmail.com


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo_name.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo_name/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo_name.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo_name/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo_name.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo_name/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo_name.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo_name/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo_name.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo_name/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/joel-%C3%B3scar-mart%C3%ADn-guti%C3%A9rrez-578ab8303
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[C++.js]: https://img.shields.io/badge/-C++-blue?logo=cplusplus
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
