// Author of Program : Fred Mose
// Date created: 10/30/2014

// This program controls a robot that sorts recycled bottles that have just been washed 
// The washed bottles arrive at the robot on a conveyor belt. The robot sorts the bottles depending on their
// color and their weight. Only the bottles that are correct color and weight are place in the outgoing belts
// The robot places broken bottles and bottles that are not clear, green, or brown in a waste container.

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


int main ()
{
	//declaration of local variables
	double weightOfSmallClearBtl = 0.0;
	double weightOfMedGreenBtl = 0.0;
	double weightOfLargeBrownBtl = 0.0;
	double weightOfWaste = 0.0;
	double totalWasteGlass = 0.0;
	double increaseInSpringLength = 0.0;
	double weightInGrams = 0.0;
	int angleOfRotation = 0;
	int numberOfSensorReadings= 0;
	int weightSensorReading = 0;
	int colorSensorReading = 0;
	int numberOfClearBtls = 0;
	int numberOfGreenBtls = 0;
	int numberOfBrownBtls = 0; 
	int line = 0;
	int tries = 0;
	char inputName[80] = {'\0'};
	char outputName[80] = {'\0'};
	ifstream inFile;
	ofstream outFile;
	bool ifStopped = false;

	//declaration of constants
	const double MAX_WEIGHT_OF_WASTE_GRAMS = 500;
	const double MAX_CLEAR_BTL_WEIGHT = 21.0;
	const double MIN_CLEAR_BTL_WEIGHT = 15.0;
	const double MAX_GREEN_BTL_WEIGHT = 41.0;
	const double MIN_GREEN_BTL_WEIGHT = 35.0;
	const double MAX_BROWN_BTL_WEIGHT = 59.0;
	const double MIN_BROWN_BTL_WEIGHT = 53.0;
	const double MAX_SPRING_LENGTH = 15.0;
	const double MIN_SPRING_LENGTH = 10.0;
	const double MAX_INCR_SPRING_LENGTH = 5.0;
	const double COEFF1 = 12.0;
	const double COEFF2 = 1000.0;
	const int MAX_SENSOR_READING = 255;
	const int MIN_SENSOR_READING = 0;
	const int MIN_PAIR_OF_SENSOR_READINGS = 0;
	const int MAX_PAIR_OF_SENSOR_READINGS = 10000;
	const int MIN_CLEAR_BTL_SENSOR_READING = 240;
	const int MAX_CLEAR_BTL_SENSOR_READING = 255;
	const int MIN_GREEN_BTL_SENSOR_READING = 150;
	const int MAX_GREEN_BTL_SENSOR_READING = 175;
	const int MIN_BROWN_BTL_SENSOR_READING = 20;
	const int MAX_BROWN_BTL_SENSOR_READING = 90;
	const int CLEAR_BTL_OUTGOING_BELT_ANGLE = 90;
	const int GREEN_BTL_OUTGOING_BELT_ANGLE = 150;
	const int BROWN_BTL_OUTGOING_BELT_ANGLE = 210;
	const int WASTE_BTL_OUTGOING_BELT_ANGLE = 270;


	// request input filename
	// open the file and check it was opened correctly
	// terminate program if opening file fails after 6 tries
	
	do
	{
		cout << "Please enter the name of the input file containing sensor readings:"<< endl;
		cin >> inputName;
		inFile.open(inputName);
		tries++;

		if (tries >5)
		{
			cerr<<"You exceeded maximum number of tries allowed for entering the input filename"<<endl;
			return 1;
		}

		if ( inFile.fail() )
		{
			cerr << "The input file "<< inputName<<" could not be opened"<< endl;
		}

			
		
	}

	while( inFile.fail());

	// request for output filename
	// open file and check it was opened correctly
	// terminate program if opening file fails after 6 tries

	do
	{
		cout <<"Please enter the name of the output file containing simulation results:"<< endl;;
		cin >> outputName;
		outFile.open(outputName);
		tries++;

		if (tries >5)
		{
			cerr<<"You exceeded maximum number of tries allowed for entering the output filename"<<endl;
			return 2;
		}

		if ( outFile.fail() )
		{
			cerr << "The output file "<< outputName<<" could not be opened"<< endl;
			
		}
	}

	while( outFile.fail());

	// check the nummber of sensor readings is an integer

	if (!(inFile>> numberOfSensorReadings))
	{
		if (inFile.eof())
		{
			cerr <<"Data file is empty"<< endl;
			inFile.close();
			outFile.close();
			return 3;
		}
		else
		{
			cerr<<" Unable to read the number of sensor readings"<< endl;
			inFile.close();
			outFile.close();
			return 4;
		}
	}

	if (numberOfSensorReadings <= MIN_PAIR_OF_SENSOR_READINGS || numberOfSensorReadings > MAX_PAIR_OF_SENSOR_READINGS)
	{
		cerr<<"The number of sensor readings is out of range"<< endl;
		inFile.close();
		outFile.close();
		return 5;
	}

	// we read the data in the input files a pair each time and convert it them into weigh and color to determine which
	// conveyor belt the robot puts them

	for(line =1; line <= numberOfSensorReadings; line++)
	{
		if( !(inFile >> weightSensorReading ))
		{
			if(inFile.eof())
			{
				cerr<<"No more data, simulation completed early on line "<<line + 1<<" of input"<< endl;
				break;
			}

			else
			{
	  			cerr<<"The number read on line "<< line + 1 <<" of your input file is not an integer,"<< endl
                    << "Please correct your data file and then rerun"<< endl;
				inFile.close();
				outFile.close();
				return 6;
			}
		}

		// checks whether we have pairs for all lines

		else if (!(inFile>> colorSensorReading))
		{
			if(inFile.eof())
			{
				cerr<< "No light sensor measurement to match final weight measurement"<< endl;
				break;
			}

			else
			{
	  			cerr<<"The number read on line "<< line + 1 <<" of your input file is not an integer,"<< endl
                    << "Please correct your data file and then rerun"<< endl;

				inFile.close();
				outFile.close();
				return 7;
			}
		
		
		}

		// calcutaing weight of bottle from sensor reading

		if( (weightSensorReading >= MIN_SENSOR_READING && weightSensorReading <= MAX_SENSOR_READING) && (colorSensorReading >= MIN_SENSOR_READING
			&& colorSensorReading <= MAX_SENSOR_READING))

		{
			increaseInSpringLength = (weightSensorReading) *  MAX_INCR_SPRING_LENGTH/MAX_SENSOR_READING;
			weightInGrams = COEFF1* increaseInSpringLength;

		}
		
		else
		{
			cerr <<"Sensor reading out of range, ignoring line "<<line +1<< " of input file"<< endl;
			continue;

		}

		// sorting the bottle and putting the to the right conveyor belt.


		// for clear small bottle robot turns 90 degrees

		if ((weightInGrams >= MIN_CLEAR_BTL_WEIGHT && weightInGrams <= MAX_CLEAR_BTL_WEIGHT) && (colorSensorReading >= MIN_CLEAR_BTL_SENSOR_READING 
			&& colorSensorReading <= MAX_CLEAR_BTL_SENSOR_READING))
		{
			angleOfRotation = CLEAR_BTL_OUTGOING_BELT_ANGLE;
			numberOfClearBtls++;

			// prints to console
			cout<<"The robot has rotated "<< angleOfRotation <<" degrees to face the clear glass conveyor belt"<< endl;
			cout<<"The robot has placed "<< numberOfClearBtls<<"  clear bottles on the conveyor belt"<< endl;

			//prints to output file
			outFile<<"The robot has rotated "<< angleOfRotation <<" degrees to face the clear glass conveyor belt"<< endl;
			outFile<<"The robot has placed " <<numberOfClearBtls <<" clear bottles on the conveyor belt"<< endl;
		}
		
		// for green medium bottles the robot turns 150 degrees

		else if ((weightInGrams >= MIN_GREEN_BTL_WEIGHT && weightInGrams <= MAX_GREEN_BTL_WEIGHT) && (colorSensorReading >= MIN_GREEN_BTL_SENSOR_READING 
			&& colorSensorReading <= MAX_GREEN_BTL_SENSOR_READING))
		{
			angleOfRotation = GREEN_BTL_OUTGOING_BELT_ANGLE;
			numberOfGreenBtls++;

			//prints to console
			cout<<"The robot has rotated "<< angleOfRotation <<" degrees to face the green glass conveyor belt"<< endl;
			cout<<"The robot has placed "<< numberOfGreenBtls<<"  green bottles on the conveyor belt"<< endl;

			//prints to output file
			outFile<<"The robot has rotated "<< angleOfRotation <<" degrees to face the green glass conveyor belt"<< endl;
			outFile<<"The robot has placed " << numberOfGreenBtls <<" green bottles on the conveyor belt"<< endl;
		}
		
		//for brown large bottles the robot turns 210 degrees

		else if ((weightInGrams >= MIN_BROWN_BTL_WEIGHT && weightInGrams <= MAX_BROWN_BTL_WEIGHT) && (colorSensorReading >= MIN_BROWN_BTL_SENSOR_READING
			&& colorSensorReading <= MAX_BROWN_BTL_SENSOR_READING))
		{
			angleOfRotation = BROWN_BTL_OUTGOING_BELT_ANGLE;
			numberOfBrownBtls++;

			//prints to console
			cout<<"The robot has rotated "<< angleOfRotation <<" degrees to face the brown glass conveyor belt"<< endl;
			cout<<"The robot has placed "<< numberOfBrownBtls<<"  brown bottles on the conveyor belt"<< endl;

			//prints to output file
			outFile<<"The robot has rotated "<< angleOfRotation <<" degrees to face the brown glass conveyor belt"<< endl;
			outFile<<"The robot has placed " <<numberOfBrownBtls <<" brown bottles on the conveyor belt"<< endl;
		}

		//for glass fragments and any other colors or weight over 60g the robot turns 270 degrees

		else
		{
			angleOfRotation = WASTE_BTL_OUTGOING_BELT_ANGLE;
			weightOfWaste += weightInGrams;
			

			cout<<"The robot has rotated "<< angleOfRotation <<" degrees to face the waste container"<< endl;
			outFile <<"The robot has rotated "<< angleOfRotation <<" degrees to face the waste container"<< endl;

			
			if(weightOfWaste > MAX_WEIGHT_OF_WASTE_GRAMS)
			{
				// prints to the console
				cout<<"A new waste container is needed"<< endl
					<<"Stop the incoming conveyor belt"<< endl;
				cout<<"A new waste container is ready for use"<< endl;

				//prints to the output file
				outFile<<"A new waste container is needed"<< endl
					   <<"Stop the incoming conveyor belt"<< endl;
				outFile<<"A new waste container is ready for use"<< endl;

				
				ifStopped = true;

				weightOfWaste = weightInGrams;

			}

			cout<<"The waste container now contains "<< fixed<< setprecision(3)<< setw(8)<< weightOfWaste<< " grams of glass"<< endl;
			outFile<<"The waste container now contains "<< fixed<< setprecision(3)<<setw(8)<< weightOfWaste<<" grams of glass"<< endl;
			totalWasteGlass += weightInGrams;
			


		}

		cout<<"The robot rotates "<<-angleOfRotation <<" degrees"<< endl; 
		outFile<<"The robot rotates "<<-angleOfRotation <<" degrees" << endl;

		if( ifStopped == true)
		{
		cout<<"Start the incoming conveyor belt"<< endl;
		outFile<<"Start the incoming conveyor belt"<< endl;

		ifStopped = false;

		}


	} 
	

	// printing the summary on the console

	cout<< endl<< endl<< endl<<endl;
	cout<< "SUMMARY"<< endl;
	cout<<"The robot processed " << numberOfClearBtls<<" clear bottles"<< endl;
	cout<<"The robot processed "<< numberOfGreenBtls<<" green bottles" << endl;
	cout<<"The robot processed "<<numberOfBrownBtls<<" brown bottles" << endl;
	cout<<"The robot processed "<< fixed<< setprecision(4) <<setw(12)<< totalWasteGlass/COEFF2 <<" kg of waste glass"<< endl;

	//printing the summary in the output file

	outFile<< endl<< endl<< endl<< endl;
	outFile<< "SUMMARY"<< endl;
	outFile<<"The robot processed " << numberOfClearBtls<<" clear bottles"<< endl;
	outFile<<"The robot processed "<< numberOfGreenBtls<<" green bottles" << endl;
	outFile<<"The robot processed "<<numberOfBrownBtls<<" brown bottles" << endl;
	outFile<<"The robot processed "<< fixed<< setprecision(4) <<setw(12)<< totalWasteGlass/COEFF2 <<" kg of waste glass"<< endl;
		
	
	
	inFile.close();
	outFile.close();


	return 0;
	}








