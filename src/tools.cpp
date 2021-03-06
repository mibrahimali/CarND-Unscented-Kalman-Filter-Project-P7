#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
  TODO:
    * Calculate the RMSE here.
  */
  VectorXd rmse(4);
	rmse << 0,0,0,0;

	// check the validity of the following inputs:
	//  * the estimation vector size should not be zero
	//  * the estimation vector size should equal ground truth vector size
	if(estimations.size() != ground_truth.size()
			|| estimations.size() == 0){
		cout << "Invalid estimation or ground_truth data" << endl;
		return rmse;
	}

	//accumulate squared residuals
	for(unsigned int i=0; i < estimations.size(); ++i){

		VectorXd residual = estimations[i] - ground_truth[i];

		//coefficient-wise multiplication
		residual = residual.array()*residual.array();
		rmse += residual;
	}

	//calculate the mean
	rmse = rmse/estimations.size();

	//calculate the squared root
	rmse = rmse.array().sqrt();

	//return the result
	return rmse;
}

VectorXd Tools::Cart2Polar(const VectorXd& x_state)
{
	VectorXd y(3);
	y << 0,0,0;

	if(x_state.size() != 5)
	{
		cout << "input size invalid, input must be of size = 5 {pos , velocity , yaw , yaw_rate)";
		return y;
	}
	//recover state parameters
	float px = x_state(0);
	float py = x_state(1);
	float v = x_state(2);
	float yaw = x_state(3);
  float yaw_rate = x_state(4);
  
	float vx = v * cosf(yaw);
	float vy = v * sinf(yaw);

	//pre-compute a set of terms to avoid repeated calculation
	float rho = sqrt(px*px+py*py);
	// check if px == 0 , this will avoid undefined behaviour of atan2 at x = 0 and y = 0 
	if (fabs(px) < 0.001)
	{
		px = 0.001;
	}
	float phi = atan2(py,px);
	
	// check rho to avoid zero division 
	if(rho < 0.0001){
		rho = 0.0001;
	}
	float rho_dot = (px*vx + py*vy) / rho;
	
	y = VectorXd(3);
	y << rho,phi,rho_dot;
	return y ;
}

VectorXd Tools::Polar2Cart(const VectorXd& x_state)
{
	VectorXd y(5);
	y << 0,0,0,0,0;
	if(x_state.size() != 3)
	{
		cout << "input size invalid, input must be of size = 3; {rho,phi,roh_dot)";
		return y;
	}
	//recover state parameters
	float rho = x_state(0);
	float phi = x_state(1);
	float rho_dot = x_state(2);
	// project polar cordinate into cartisian 
	float px = rho * cosf(phi);
	float py = rho * sinf(phi);
	float vx = rho_dot * cosf(phi);
	float vy = rho_dot * sinf(phi);
	float v = sqrtf((vx*vx)+(vy*vy));
  y = VectorXd(5);
	y << px,py,v,0,0;
	return y;
}