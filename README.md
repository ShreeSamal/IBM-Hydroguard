[![License](https://img.shields.io/badge/License-Apache2-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0) [![Community](https://img.shields.io/badge/Join-Community-blue)](https://developer.ibm.com/callforcode/solutions/projects/get-started/) [![Website](https://img.shields.io/badge/View-Website-blue)](https://hydroguard.onrender.com)

# HydroGuardian

- [Project summary](#project-summary)
  - [The issue we are hoping to solve](#the-issue-we-are-hoping-to-solve)
  - [How our technology solution can help](#how-our-technology-solution-can-help)
  - [Our idea](#our-idea)
- [Technology implementation](#technology-implementation)
  - [IBM AI service(s) used](#ibm-ai-services-used)
  - [Other IBM technology used](#other-ibm-technology-used)
  - [Solution architecture](#solution-architecture)
- [Presentation materials](#presentation-materials)
  - [Solution demo video](#solution-demo-video)
  - [Project development roadmap](#project-development-roadmap)
- [Additional details](#additional-details)
  - [How to run the project](#how-to-run-the-project)
  - [Live demo](#live-demo)
- [About this template](#about-this-template)
  - [Contributing](#contributing)
  - [Versioning](#versioning)
  - [Authors](#authors)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)


## Project summary

### The issue we are hoping to solve
The issue we are hoping to solve revolves around the critical concerns of water quality and distribution. The country has 18 percent of the world’s population, but only 4 percent of its water resources, making it among the most water-stressed in the world. Widespread water contamination is a daunting challenge in India with 1,95,813 habitations in the country reported to have poor water quality, posing serious health risks to the population. Pollution in India has led to more than 2.3 million premature deaths in 2019, according to a Lancet study. More than half a million were caused by water pollution.

### How our technology solution can help
Our system uses various sensors, like pH and turbidity, for real-time water quality monitoring in societies and distribution plants. Machine learning adjusts quality maintenance based on geographic locations. Real-time water flow sensors to detect issues like leaks or tampering, ensuring continuous assessment and problem detection, vital for safeguarding water quality.

### Our idea
**Revolutionizing Water Quality Management through IoT and Machine Learning**

In a world grappling with critical challenges such as water scarcity and declining water quality, we present a visionary solution that harnesses the power of cutting-edge technology and innovative data analysis. Our idea revolves around the deployment of an integrated system of Internet of Things (IoT) devices equipped with state-of-the-art sensors to monitor and manage water quality throughout the entire supply chain. This comprehensive approach not only ensures the delivery of safe and clean drinking water but also tackles issues like leaks, tampering, and accountability within the water distribution infrastructure.

More detail is available in [this document](./docs/DESCRIPTION.md).

## Technology implementation

### IBM AI service(s) used

[IBM WatsonX](https://www.ibm.com/watsonx) -

IBM WatsonX is a powerful suite of AI tools and services offered by IBM. In the context of this project, it is employed for data analysis. This means that the system utilizes advanced machine learning and artificial intelligence algorithms provided by WatsonX to make sense of the vast amount of data collected by the IoT sensors.

IBM WatsonX can analyze complex data sets, identify patterns, trends, anomalies, and correlations within the water quality and flow data. By leveraging AI, the project can gain deeper insights into the state of the water supply and make informed decisions based on this analysis. For instance, it can help in optimizing the parameters for maintaining water quality based on geographic regions, identifying potential issues, and even predicting future water quality problems.

### Other IBM technology used
Data generated by the IoT sensors, as well as the results of the AI data analysis, need a secure and scalable storage solution. In this case, the project has chosen to use IBM Cloud, IBM's cloud computing platform, for this purpose. IBM Cloud offers a reliable and flexible cloud infrastructure where data can be securely stored, managed, and accessed.

The use of IBM Cloud ensures that the data collected from the IoT devices is stored in a centralized and easily accessible location. This centralized storage enables real-time monitoring and analysis of water quality data, making it accessible to stakeholders, authorities, and the project team. Furthermore, it provides a robust and scalable infrastructure for managing the growing amount of data generated by the IoT devices, supporting the long-term sustainability and effectiveness of the system.

### Solution architecture

Diagram of the flow of our solution:

![Solution Architecture](https://github.com/ShreeSamal/HydroGuard/assets/101418323/085b709e-dd91-4b2b-9deb-b5f3cdaa9934)

## Presentation materials


### Solution demo video

[![Watch the video](https://youtu.be/HG0L4r0e8qE)

### Project development roadmap

The project currently does the following things.

  - Complaint System for User:
    - Streamlined user-friendly complaint system with prompt resolution and feedback.

  - Admin Dashboard for Society Head:
    - Comprehensive admin dashboard enabling real-time monitoring of society water quality for effective management.

  - Admin Dashboard for Government Body:
    - Comprehensive admin dashboard enabling real-time monitoring of overall locality's water quality for effective management.

  - Water Quality Analysis Using different sensors
    1. pH Sensor
    2. Turbidity Sensor
    3. TDS Sensor
    4. Temperature Sensor
    5. Water Flow Sensor

In the future we plan for...

  **Expansion to Other Regions**: Initially designed to address water quality issues in India, HydroGuardian can be expanded to other regions and countries facing similar water quality and scarcity challenges. This would require adapting the system to local conditions and regulatory requirements.

  **Mobile Application for Public Reporting**: Develop a mobile application that allows the general public to report water quality concerns or issues they observe. This can further engage the community in safeguarding water quality.

  **Water Quality Certification**: Introduce a certification system that rewards regions or communities with consistently high water quality standards. Such certification can be used for marketing purposes and attract investment and tourism.



## Additional details

### How to run the project
**1. Create a virtual environment:**

  - pip install virtualenv
  - virtualenv env

**2. Activate environment:**

  - env\Scripts\activate

**3. Install requirements:**

  - pip install -r requirements.txt 

**4. Set environment variable for the flask application:**
  - set FLASK_APP=app.py

**5. Set the FLASK_ENV environment variable to "development:**
  - set FLASK_ENV=development

**6. Run the application:**
  - flask run

### Live Demo

URL: https://hydroguard.onrender.com

Demo Credentials -
 - For Society
   - email- "kaushal@mail.com"
   - password- "kaushal"
 - For Government
   - email- "shree@mail.com"
   - password- "shree"


## About this template

### Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

### Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags).

### Authors

- [**Shaun Dsouza**](https://github.com/Shaun-Dsouza-717)
- [**Kaushal Poojary**](https://github.com/Kaushal-Poojary)
- [**Shree Samal**](https://github.com/ShreeSamal)
- [**Aaman Bhowmick**](https://github.com/AamanBhowmick)
- [**Manali Bhave**](https://github.com/ManaliBhave)

### License

This project is licensed under the Apache 2 License - see the [LICENSE](LICENSE) file for details.

### Acknowledgments

- We acknowledge the guidance of our teachers Prof. Bincy Ivin, Prof. Rohini Sawant and Prof. Ravita Mishra 
- Based on [Billie Thompson's README template](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2).
