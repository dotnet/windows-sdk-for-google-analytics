#pragma once
//
// Dimensions.h
// Declaration of the Dimensions structure .
//

namespace GoogleAnalytics
{
	/// <summary>
	/// Dimensions in pixels.
	/// </summary>
	public value struct Dimensions sealed
	{	
	public:
		/// <summary>
		/// Creates a new object to store dimensions.
		/// </summary>
		/// <param name="width">The width in pixels.</param>
		/// <param name="height">The height in pixels.</param>
		/*Dimensions(int width, int height)
		{
			Width = width; 
			Height = height; 
		}*/


		/// <summary>
		/// Gets the width in pixels.
		/// </summary>
		float Width; 


		/// <summary>
		/// Gets the height in pixels.
		/// </summary>
		float Height;		
	};
}
